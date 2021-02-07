#include "messenger.h"
#include "sender.h"
#include "utils.h"
#include "masker.h"

#include <iostream>

Mere::Message::Messenger::~Messenger()
{
}

Mere::Message::Messenger::Messenger(const char *path, QObject *parent)
    : QObject(parent),
      m_space(path, 4, 4096),
      m_notifier(new Notifier())
{
    if(!Utils::isValidName(path))
        throw std::invalid_argument("Invalid shared message space name.");

    // someone send a request
    connect(notifier(), SIGNAL(bleep(pid_t, int)), this, SLOT(catched(pid_t, int)));

    srand (time(NULL));
}

Mere::Message::Messenger::Messenger(const std::string &path, QObject *parent)
    : Messenger(path.c_str(), parent)
{
}

int Mere::Message::Messenger::bind()
{
    return m_space.bind();
}

int Mere::Message::Messenger::join()
{
    int err = m_space.join();
    if (!err)
    {
        send(JOIN);
        ready(true);
    }

    return err;
}

int Mere::Message::Messenger::done()
{
    return m_space.done();
}

pid_t Mere::Message::Messenger::origin() const
{
    return m_space.origin();
}

int Mere::Message::Messenger::ping(int signal, int target)
{
    return write(signal, target, Method::PING);
}

int Mere::Message::Messenger::pong(int signal, int target)
{
    return write(signal, target, Method::PONG);
}

int Mere::Message::Messenger::send(const char *data)
{
    return send(data, origin());
}

int Mere::Message::Messenger::send(const char *data, const pid_t target)
{
    const std::vector<int> targets({target});
    return this->send(data, targets);
}

int Mere::Message::Messenger::send(const char *data, const std::vector<pid_t> &targets)
{
    if (!ready()) return 1;

    Sender *sender = static_cast<Sender *>(this->parent());
    int signal = m_space.origin() == sender->whoami() ? SIGUSR1 : SIGUSR2;
    pid_t target = targets.size() == 1 ? targets.at(0) : 0;

    mid_t id;
    int err = write(signal, target, data, &id);
    if(err)
    {
        qDebug() << "Failed to write message to space.";
        return err;
    }

    for(const pid_t &target : targets)
        notify(signal, target, {Method::POST, id});

    return 0;
}

int Mere::Message::Messenger::send(const Method &method)
{
    return send(method, origin());
}

int Mere::Message::Messenger::send(const Method &method, const pid_t target)
{
    const std::vector<pid_t> targets({target});

    return this->send(method, targets);
}

int Mere::Message::Messenger::send(const Method &method, const std::vector<pid_t> &targets)
{
    Sender *sender = static_cast<Sender *>(this->parent());
    int signal = m_space.origin() == sender->whoami() ? SIGUSR1 : SIGUSR2;

    pid_t target = targets.size() == 1 ? targets.at(0) : 0;
    write(signal, target, method);
    for(const pid_t &target : targets)
        notify(signal, target, method);

    return 0;
}

int Mere::Message::Messenger::write(int signal, int target, Method method)
{
    Q_UNUSED(signal);

    Command command;
    command.method = method;
    command.source = getpid();
    command.target = target;

    m_space.set(command);
    return 0;
}

int Mere::Message::Messenger::write(int signal, int target, const char *data, mid_t *id)
{
    if ((strlen(data) + 1) > (sizeof(((Message *)0)->_dblock)) - sizeof (((Message *)0)->_dblock.type))
        return 1;

    Message message;

    // id
    message.id = ID::next(*this);

    // meta block
    message.signal  = signal;
    message.channel = 0;

    // get the number of connection and set it here
    // when message sent for all.
    message.clients = (target == 0) ? 0 : 1;

    // route block
    message._rblock.source = getpid();
    message._rblock.target = target;

    // data block
    message._dblock.type = MimeType::TEXT;
    memcpy (message._dblock.data, data, strlen(data) + 1 );

    int err = m_space.set(message, [](const Message *msg)->bool {
                            /* target(s) seen the message - isSeen? */
                            return msg->clients == 0;
                          });

    if(!err && id) *id = message.id ;

    qDebug() << "Message with id: " << message.id << " written successfully.";

    return err;
}

bool Mere::Message::Messenger::ready() const
{
    return m_space.ready();
}

void Mere::Message::Messenger::ready(bool ready)
{
    m_space.ready(ready);
}

Mere::Message::Notifier* Mere::Message::Messenger::notifier() const
{
    return m_notifier;
}

void Mere::Message::Messenger::notify()
{
    Sender *sender = static_cast<Sender *>(this->parent());
    int signal = m_space.origin() == sender->whoami() ? SIGUSR1 : SIGUSR2;

    notify(signal);
}

void Mere::Message::Messenger::notify(int signal)
{
    Command command = m_space.command();
    notify(signal, command.source);
}

void Mere::Message::Messenger::notify(int signal, int target)
{
    Command command = m_space.command();
    notify(signal, target, {Method::ACKN, command.method});
}

void Mere::Message::Messenger::notify(int signal, int target, Method method)
{
    Masker masker;

    int mask = masker.mask({method, 0});

    m_notifier->send(signal, target, mask);
}

void Mere::Message::Messenger::notify(int signal, int target, std::pair<method_t, mid_t> pair) const
{
    Masker masker;

    int mask = masker.mask({pair.first, pair.second});
    m_notifier->send(signal, target, mask);
}


int Mere::Message::Messenger::ackn(pid_t target, Method method)
{
    Sender *sender = static_cast<Sender *>(this->parent());
    int signal = m_space.origin() == sender->whoami() ? SIGUSR1 : SIGUSR2;
    notify(signal, target, {Method::ACKN, method});

    return 0;
}

bool Mere::Message::Messenger::exists(const mid_t id) const
{
    int numberOfMessages = m_space.numberOfUnits();
    for (int index = 0; index < numberOfMessages; index++)
    {
        Message *message = m_space.get(index);
        if (message && message->id == id)
            return true;
    }

    return false;
}

Mere::Message::Message* Mere::Message::Messenger::get(mid_t id) const
{
    qDebug() << "Going to fetch message with id:" << id << &m_space;
    int numberOfMessages = m_space.numberOfUnits();
    for (int index = 0; index < numberOfMessages; index++)
    {
        Message *msg = m_space.get(index);
        if (!msg) continue;
        qDebug() << msg << msg->id;
        if (msg->id == id)
            return msg;
    }

    return nullptr;
}

bool Mere::Message::Messenger::seen(mid_t id) const
{
    Message *message = this->get(id);
    if (!message)
        return false;

    return this->seen(message);
}

bool Mere::Message::Messenger::seen(Message *message) const
{
    if( message->clients == 0)
        return false;

    --message->clients;

    // Notify the message author...
    Sender *sender = static_cast<Sender *>(this->parent());
    int signal = m_space.origin() == sender->whoami() ? SIGUSR1 : SIGUSR2;
    notify(signal, message->_rblock.source, {Method::SEEN, message->id});

    return true;
}

void Mere::Message::Messenger::catched(pid_t who, int what)
{
    //qDebug() << "Catched...." << m_space.origin() << what;
    Masker masker;
    std::pair<method_t, uchar> pair = masker.unmask(what);

    Method method = (Method) pair.first;

    //Command command = m_space.command();

    if (method == JOIN)
        emit join(who);
    else if (method == PING)
        emit ping(who);
    else if (method == PONG)
        emit pong(who);
    else if (method == POST)
    {
        mid_t id = pair.second;
        // retrieve message
        Message *message = this->get(id);
        if(!message)
        {
            std::cout << "No message found with id:" << id << std::endl;
            return;
        }

        // copy
        Message copy = *message;

        // seen
        seen(message);

        // read the message.....
        emit this->post(id);
        emit this->message(copy._dblock.data);
        emit this->message(copy);
    }
    else if (method == SEEN)
        emit seen(who, pair.second);
    else if (method == ACKN)
        emit ackn(who, pair.second);

    // avoid acknowledging the ACKN knock
    if(method != ACKN)
        ackn(who, method);

//    emit call(mask, command.source);
//    emit message(*m_space.message());
}


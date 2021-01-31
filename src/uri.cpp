#include "uri.h"

std::vector<std::string> Mere::Message::Uri::schemas = {"mms"};

Mere::Message::Uri::Uri(const char *uri, QObject *parent)
    : QObject(parent)
{
    if(!isValid(uri))
        throw std::invalid_argument("Invalid uri");

    m_uri = QUrl(uri);
}

Mere::Message::Uri::Uri(const std::string &uri, QObject *parent)
    : QObject(parent)
{
    if(!isValid(uri))
        throw std::invalid_argument("Invalid uri");

    m_uri = QUrl(uri.c_str());
}

Mere::Message::Uri::Uri(const QUrl &uri, QObject *parent)
    : QObject(parent)
{
    if(!isValid(uri))
        throw std::invalid_argument("Invalid uri");

    m_uri = uri;
}

const std::string Mere::Message::Uri::schema() const
{
    return m_uri.scheme().toStdString();
}

const std::string Mere::Message::Uri::server() const
{
    return m_uri.host().toStdString();
}

const std::string Mere::Message::Uri::service() const
{
    return m_uri.path().remove(0, 1).toStdString();
}

const std::string Mere::Message::Uri::fqpath() const
{
    QString uri(m_uri.url(QUrl::RemoveScheme | QUrl::RemoveQuery | QUrl::StripTrailingSlash));
    uri = uri.remove(0, 1);

    return uri.toStdString();
}

//static
bool Mere::Message::Uri::isValid(const char *uri)
{
    return isValid(std::string(uri));
}

//static
bool Mere::Message::Uri::isValid(const std::string &uri)
{
    return isValid(QString(uri.c_str()));
}

//static
bool Mere::Message::Uri::isValid(const QString &uri)
{
    return isValid(QUrl(uri));
}

//static
bool Mere::Message::Uri::isValid(const QUrl uri)
{
    if(!uri.isValid())
        return false;

    QString schema = uri.scheme();
    auto result = std::find(std::begin(schemas), std::end(schemas), schema.toStdString());
    if(result == std::end(schemas))
        return false;

    return true;
}

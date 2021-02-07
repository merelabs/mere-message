#include "config.h"

Mere::Message::Config::Config(QObject *parent) : QObject(parent)
{

}

QVariant Mere::Message::Config::get(const std::string &key) const
{
    QVariant value;

    auto it = m_config.find(key);
    if(it != m_config.end())
        value = it->second;

    return value;
}

void Mere::Message::Config::set(const std::string &key, const QVariant &value)
{
    m_config[key] = value;
}

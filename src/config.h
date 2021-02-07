#ifndef MERE_MESSAGE_CONFIG_H
#define MERE_MESSAGE_CONFIG_H

#include "global.h"
#include <QObject>
#include <QVariant>

#include <string>
#include <unordered_map>

namespace Mere
{
namespace Message
{

class MERE_MESSAGE_LIB_SPEC Config : public QObject
{
    Q_OBJECT
public:
    explicit Config(QObject *parent = nullptr);

    QVariant get(const std::string &key) const;
    void set(const std::string &key, const QVariant &value);

private:
    std::unordered_map<std::string, QVariant> m_config;
};

}
}
#endif // MERE_MESSAGE_CONFIG_H

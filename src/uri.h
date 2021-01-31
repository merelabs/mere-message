#ifndef URL_H
#define URL_H

#include <QUrl>
#include <QObject>

namespace Mere
{
namespace Message
{

class Uri : public QObject
{
    Q_OBJECT
public:
    explicit Uri(const char *uri, QObject *parent = nullptr);
    explicit Uri(const std::string &uri, QObject *parent = nullptr);
    explicit Uri(const QUrl &uri, QObject *parent = nullptr);

    const std::string schema() const;
    const std::string server() const;
    const std::string service() const;

    const std::string fqpath() const;

    static bool isValid(const char *uri);
    static bool isValid(const std::string &uri);
    static bool isValid(const QString &uri);
    static bool isValid(const QUrl uri);

    static std::vector<std::string> schemas;

signals:

private:
    QUrl m_uri;
};

}
}

#endif // URL_H

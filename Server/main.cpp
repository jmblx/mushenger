#include <QTcpServer>
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QByteArray>
#include <QDebug>
#include <QCoreApplication>
#include <QDir>
#include <QCoreApplication>
#include "server.h"


class UserServer : public QTcpServer
{
    Q_OBJECT

public:
    UserServer(QObject *parent = nullptr) : QTcpServer(parent)
    {
        loadUserData();
    }

    void startServer()
    {
        if (this->listen(QHostAddress::Any, 12345)) {
            qDebug() << "Server started...";
        } else {
            qDebug() << "Server failed to start!";
        }
    }

protected:
    void incomingConnection(qintptr socketDescriptor) override
    {
        QTcpSocket *socket = new QTcpSocket(this);
        socket->setSocketDescriptor(socketDescriptor);
        connect(socket, &QTcpSocket::readyRead, this, &UserServer::onReadyRead);
        connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);
        qDebug() << "New connection established!";
    }

private slots:
    void onReadyRead()
    {
        QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
        QByteArray data = socket->readAll();

        QJsonDocument requestDoc = QJsonDocument::fromJson(data);
        if (!requestDoc.isObject()) {
            socket->write("Invalid JSON format.");
            socket->flush();
            return;
        }

        QJsonObject request = requestDoc.object();
        QString login = request.value("login").toString();
        QString password = request.value("password").toString();

        if (login.isEmpty() || password.isEmpty()) {
            socket->write("Login and password are required.");
            socket->flush();
            return;
        }

        QJsonObject response;

        if (userData.contains(login)) {
            if (userData[login] == password) {
                response["status"] = "success";
                response["message"] = "Login successful.";
            } else {
                response["status"] = "error";
                response["message"] = "Incorrect password.";
            }
        } else {
            userData[login] = password;
            saveUserData();
            response["status"] = "success";
            response["message"] = "Registration successful.";
        }

        QJsonDocument responseDoc(response);
        socket->write(responseDoc.toJson());
        socket->flush();
    }

private:
    QJsonObject userData;

    void loadUserData()
    {
        QFile file(QDir::currentPath() + "/users.json");


        if (file.open(QIODevice::ReadOnly)) {
            QByteArray jsonData = file.readAll();
            file.close();
            QJsonDocument doc = QJsonDocument::fromJson(jsonData);
            if (doc.isObject()) {
                userData = doc.object();
            }
        }
    }

    void saveUserData()
    {
        QString filePath = QDir::currentPath() + "/users.json";
        QFile file(filePath);
        qDebug() << "Saving to file:" << filePath;

        if (file.open(QIODevice::WriteOnly)) {
            QJsonDocument doc(userData);
            file.write(doc.toJson());
            file.close();
            qDebug() << "Data saved successfully to:" << filePath;
        } else {
            qDebug() << "Failed to open file for writing:" << file.errorString();
        }
    }

};

int main(int argc, char *argv[])
{
    QDir dir;
    if (!dir.exists("chat_avs")) {
        dir.mkpath("chat_avs");
        qDebug() << "Created directory chat_avs for storing chat avatars.";
    }
    QCoreApplication a(argc, argv);

    Server server;
    server.startServer(12345);

    return a.exec();
}

#include "main.moc"

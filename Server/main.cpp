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

        // Проверка наличия пользователя
        if (userData.contains(login)) {
            // Пользователь найден, проверяем пароль
            if (userData[login] == password) {
                response["status"] = "success";
                response["message"] = "Login successful.";
            } else {
                response["status"] = "error";
                response["message"] = "Incorrect password.";
            }
        } else {
            // Регистрация нового пользователя
            userData[login] = password;
            saveUserData();
            response["status"] = "success";
            response["message"] = "Registration successful.";
        }

        // Отправка ответа клиенту
        QJsonDocument responseDoc(response);
        socket->write(responseDoc.toJson());
        socket->flush();
    }

private:
    QJsonObject userData;

    void loadUserData()
    {
        QFile file(QDir::currentPath() + "/users.json"); // Прямой путь до текущего каталога


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
        qDebug() << "Saving to file:" << filePath; // Вывод пути для проверки

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
    QCoreApplication a(argc, argv);

    Server server;
    server.startServer(12345); // Изменено с Connect на startServer

    return a.exec();
}

#include "main.moc"

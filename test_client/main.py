import socket
import json

def send_request(login, password):
    # Настройки подключения к серверу
    host = '127.0.0.1'  # локальный хост
    port = 12345        # порт, на котором работает сервер

    # Подключение к серверу
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client_socket.connect((host, port))

    # Формируем JSON запрос
    request = {
        "login": login,
        "password": password
    }
    request_data = json.dumps(request).encode('utf-8')

    # Отправка данных
    client_socket.sendall(request_data)

    # Получение ответа
    response_data = client_socket.recv(1024)
    response = json.loads(response_data.decode('utf-8'))
    print("Ответ от сервера:", response)

    # Закрытие соединения
    client_socket.close()

# Примеры тестов
print("Тест регистрации:")
send_request("user111", "password11")  # Должен зарегистрировать нового пользователя

print("Тест авторизации:")
send_request("user1", "password1")  # Должен авторизовать существующего пользователя

print("Тест неверного пароля:")
send_request("user1", "wrong_password")  # Должен вернуть ошибку о неверном пароле

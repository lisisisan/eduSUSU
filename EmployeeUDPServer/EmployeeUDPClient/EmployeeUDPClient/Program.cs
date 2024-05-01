using System;
using System.Net;
using System.Net.Sockets;
using System.Text;

class EmployeeUDPClient
{
    public static void Main(string[] args)
    {
        // Устанавливаем IP-адрес и порт сервера по умолчанию
        string ipAddress = "127.0.0.1";
        Int32 port = 13000;

        // Если IP-адрес передан как параметр командной строки, используем его
        if (args.Length > 0)
            ipAddress = args[0];

        // Создаем экземпляр класса UdpClient для отправки и получения данных по UDP
        UdpClient udpClient = new UdpClient(ipAddress, port);
        
        // Создаем объект IPEndPoint для хранения информации о удаленном хосте
        IPEndPoint remoteEndPoint = null;

        // Бесконечный цикл для взаимодействия с пользователем
        while (true)
        {
            // Запрашиваем имя сотрудника у пользователя
            Console.Write("Name: ");
            string name = Console.ReadLine();
            
            // Если пользователь ввел пустую строку, выходим из цикла
            if (name == "") 
                break;
            
            // Преобразуем имя сотрудника в массив байт для отправки по сети
            byte[] sendData = Encoding.ASCII.GetBytes(name);
            
            // Отправляем данные на сервер
            udpClient.Send(sendData, sendData.Length);
            
            // Принимаем данные от сервера и сохраняем информацию об удаленном хосте
            byte[] receiveData = udpClient.Receive(ref remoteEndPoint);
            
            // Преобразуем полученные данные в строку и выводим на консоль
            string job = Encoding.ASCII.GetString(receiveData);
            Console.WriteLine(job);
        }
    }
}

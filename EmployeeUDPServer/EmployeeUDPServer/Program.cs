using System;
using System.Net;
using System.Text;
using System.Net.Sockets;
using System.Collections.Generic;

class EmployeeUDPServer
{
    // Наш "эмулятор базы данных"
    // Словарь, в котором мы будем хранить всю информацию
    static Dictionary<string, string> employees =
        new Dictionary<string, string>()
    {
        {"john", "manager"},
        {"jane", "steno"},
        {"jim", "clerk"},
        {"jack", "salesman"}
    };

    public static void Main()
    {
        // Устанавливаем порт, на котором будет работать сервер
        Int32 port = 13000;
        
        // Создаем экземпляр класса UdpClient для прослушивания входящих сообщений по протоколу UDP
        UdpClient udpc = new UdpClient(port);
        
        // Выводим сообщение о запуске сервера и используемом порту
        Console.WriteLine("Server started, servicing on port " + port.ToString());
        
        // Создаем объект IPEndPoint для хранения информации о удаленном хосте
        IPEndPoint? ep = null;
        
        // Бесконечный цикл для обработки входящих сообщений
        while (true)
        {
            // Принимаем данные от клиента и сохраняем информацию о удаленном хосте
            byte[] receiveData = udpc.Receive(ref ep);
            
            // Преобразуем полученные байты в строку (имя сотрудника)
            string name = Encoding.ASCII.GetString(receiveData);
            
            // Получаем должность сотрудника из словаря
            string job = employees[name];
            
            // Если сотрудника с таким именем нет, возвращаем сообщение "No such employee"
            if (job == null) 
                job = "No such employee";
            
            // Преобразуем ответ в байтовый массив
            byte[] sendData = Encoding.ASCII.GetBytes(job);
            
            // Отправляем ответ клиенту по UDP
            udpc.Send(sendData, sendData.Length, ep);
        }
    }
}

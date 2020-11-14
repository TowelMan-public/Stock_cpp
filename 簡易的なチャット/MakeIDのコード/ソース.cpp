#include <iostream>
using namespace std;

#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"lib64\\vs14\\mysqlcppconn8.lib")
#pragma comment(lib,"lib64\\vs14\\mysqlcppconn.lib")

#include <windows.h>
#include <math.h>
#include <string>
#include <atlstr.h>
#include <xstring>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <mysql_error.h>
#include <cppconn/Statement.h>
#include <cppconn/ResultSet.h>

//文字列変換
std::string WStringToString(std::wstring istr) {
    int size = WideCharToMultiByte(CP_OEMCP, 0, istr.c_str(), -1, (char*)NULL, 0, NULL, NULL);
    char* ostr = new char[size];
    WideCharToMultiByte(CP_OEMCP, 0, istr.c_str(), -1, ostr, size, NULL, NULL);
    std::string rtn = ostr;
    delete[] ostr;
    return rtn;
}

std::wstring StringToWString(std::string istr) {
    size_t size;
    wchar_t* ostr = new wchar_t[istr.length() + 2];
    mbstowcs_s(&size, ostr, istr.length() + 1, istr.c_str(), _TRUNCATE);
    std::wstring rtn = ostr;
    delete[] ostr;
    return rtn;
}

template <typename ... Args>
std::wstring format(const std::wstring fmt, Args ... args)
{
    CString rtn;
    rtn.Format(fmt.c_str(),args...);
    return rtn.GetBuffer();
}

#define IPhost /*DBのあるIPアドレス*/""

int main(int argc, char* argv[]) {
    string rname = "chat_root";
    string rpass = "f236weg";
    sql::mysql::MySQL_Driver* driver;//データベースへのドライバー
    sql::Statement* stmt = nullptr;
    sql::ResultSet* res = nullptr;
    FILE* fp;
    if (argc != 3) {
        cout << -1 << endl;
        return -1;
    }
    wstring name = StringToWString(argv[1]);
    wstring pass = StringToWString(argv[2]);
    try {
        int MyUserID;
        driver = sql::mysql::get_mysql_driver_instance();//ドライバーの生成
        auto con = driver->connect(IPhost, rname.c_str(), rpass.c_str());//ルートアカウント（アカウントを登録するためのやつ）に接続
        stmt = con->createStatement();        
        stmt->execute("SET character_set_connection = \'utf8\'");
        wstring rsql1 = (L"SELECT val FROM chat.variable WHERE name = \'userid\'");
        string sqlM = WStringToString(rsql1);
        res = stmt->executeQuery(sqlM.c_str());
        res->next();
        MyUserID = res->getInt(1);
        wstring sql1 = (L"UPDATE chat.variable SET val = val + 1 WHERE name = \'userid\'");
        sqlM = WStringToString(sql1);
        stmt->execute(sqlM.c_str());
        wstring sql2 = (L"INSERT INTO chat.useri VALUES(%d,\'%ls\')");
        sql2 = format(sql2, MyUserID, name);
        sqlM = WStringToString(sql2);
        stmt->execute(sqlM.c_str());
        wstring sql3 = (L"CREATE USER \'chat-%d\'@\'%%\' IDENTIFIED BY \'%ls\'");
        sql3 = format(sql3, MyUserID, pass);
        sqlM = WStringToString(sql3);
        stmt->execute(sqlM.c_str());
        wstring sql4 = (L"GRANT ALL ON chat.* TO \'chat-%d\'@\'%%\' IDENTIFIED BY \'%ls\'");
        sql4 = format(sql4, MyUserID, pass);
        sql::SQLString test;
        
        sqlM = WStringToString(sql4);
        stmt->execute(sqlM.c_str());
        if (con != nullptr) {
            con->close();
            delete con;
            con = nullptr;
        }
        if (res != nullptr) {
            res->close();
            delete res;
            res = nullptr;
        }
        if (stmt != nullptr) {
            stmt->close();
            delete stmt;
            stmt = nullptr;
        }
        if (driver != nullptr) {
            driver->threadEnd();
            driver->threadInit();
            delete driver;
            driver = nullptr;
        }
        fopen_s(&fp,"id.txt", "w");
        if (fp != NULL) {
            fprintf(fp, to_string(MyUserID).c_str());
            fclose(fp);
            return MyUserID;
        }
        else
            return -1;
    }
    catch (sql::SQLException e) {
        string t1, t2, t3;
        int t4;
        t1 = e.getSQLState();
        t2 = e.getSQLStateCStr();
        t3 = e.what();
        t4 = e.getErrorCode();
        cout << -2 << endl;
        return -2;
    }
    cout << -1 << endl;
    return -1;
}
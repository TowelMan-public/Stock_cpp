#include "chat.h"

#include < process.h >
#include <cstdlib>

#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"lib64\\vs14\\mysqlcppconn8.lib")
#pragma comment(lib,"lib64\\vs14\\mysqlcppconn.lib")
#pragma conment(lib,"lib64\\vs14\\libcrypto.lib")
#pragma conment(lib,"lib64\\vs14\\libssl.lib")
#pragma conment(lib,"lib64\\vs14\\mysqlcppconn.lib")
#pragma conment(lib,"lib64\\vs14\\mysqlcppconn8.lib")
#pragma conment(lib,"lib64\\vs14\\mysqlcppconn8-static.lib")
#pragma conment(lib,"lib64\\vs14\\mysqlcppconn-static.lib")

#pragma conment(dll,"lib64\\vs14\\mysqlcppconn8.lib")

//������ϊ�
std::string WStringToString(std::wstring istr) {
	int size= WideCharToMultiByte(CP_OEMCP, 0, istr.c_str(), -1, (char*)NULL, 0, NULL, NULL);
	char* ostr = new char[size];
	WideCharToMultiByte(CP_OEMCP, 0, istr.c_str(), -1, ostr, size, NULL, NULL);
	std::string rtn = ostr;
	delete[] ostr;
	return rtn;
}

std::wstring StringToWString(std::string istr) {
	size_t size;
	wchar_t* ostr = new wchar_t[istr.length()+2];
	mbstowcs_s(&size, ostr, istr.length() + 1, istr.c_str(), _TRUNCATE);
	std::wstring rtn = ostr;
	delete[] ostr;
	return rtn;
}

//������̃t�H�[�}�b�g��
template <typename ... Args>
std::wstring format(const std::wstring fmt, Args ... args)
{
	CString rtn;
	rtn.Format(fmt.c_str(), args...);
	return rtn.GetBuffer();
}

namespace DB {
	//�f�[�^�x�[�X�Ŏg���萔�Q
	const std::string IPhost = ""/*DB�̂���IP�A�h���X������*/;
	//private�ȃO���[�o���ϐ�
	static sql::mysql::MySQL_Driver* driver = nullptr;//�f�[�^�x�[�X�ւ̃h���C�o�[
	static sql::Statement* stmt = nullptr;
	static sql::ResultSet* res = nullptr;
	static sql::Connection* con = nullptr;
	static sql::ResultSet* ChatRes = nullptr;
	static int MyUserID = -1;//���[�U�[ID�@-1�Ȃ�o�^����Ă��Ȃ� 
	//�֐�
	void End(void);
	bool HaveMyID(void);
	void GetMyUeserID_ByFile(void);
	bool GetIsLogin(void);
	bool GetIsGroupByID(int talkID);
	void SendChat(int talkID, std::wstring text);
	Accnt GetGroupByID(int talkID);
	Accnt GetUserByID(int talkID);
	AccountS GetUser(void);
	int GetTalkByUser(int userID);
	AccountS GetGroup(void);
	int GetTalkByGroup(int groupID);
	void Load(int talkID);
	bool GetChatIsMe(void);
	std::wstring GerChatUser(void);
	std::wstring GetChat(void);
	void NextChat(void);
	void Login(std::wstring pass);
	void MakeAccount(std::wstring name, std::wstring pass);
	void AddUser(int id);
	void MakeGroup(std::wstring name);
	void AddGroup(int id);
	AccountS GetDuser(void);
	void AcceptUser(int userID);
	void ResetResult(sql::ResultSet* &res);
	void ChatEnd(void);
	int GetChatCount();
	AccountS GetDgroup(int groupID);
	void AcceptGroup(int groupID, int userID);
}

void DB::End(void) {
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
	if (ChatRes != nullptr) {
		ChatRes->close();
		delete ChatRes;
		ChatRes = nullptr;
	}
	if (driver != nullptr) {
		driver->threadEnd();
		driver->threadInit();
		delete driver;
		driver = nullptr;
	}
}

void DB::ResetResult(sql::ResultSet*& res){
	res->close();
	delete res;
	res = nullptr;
}

bool DB::HaveMyID(void) {//ID�̃��[�h������
	FILE* fp;
	if (MyUserID == -1) {
		fopen_s(&fp, "id.txt", "r");
		if (fp != NULL) {
			fclose(fp);
			return true;
		}
		return false;
	}
	else {//���ɂ���
		return true;
	}
}

void DB::GetMyUeserID_ByFile(void) {
	FILE* fp;
	char num[20];
	if (MyUserID == -1) {
		try {
			fopen_s(&fp, "id.txt", "r");
			if (fp != NULL) {
				rewind(fp);
				fgets(num, 20, fp);
				fclose(fp);
				MyUserID = atoi(num);
			}
		}
		catch (...) {
			//
		}
	}
}

bool DB::GetIsLogin(void) {
	if (driver == nullptr)//���O�C������Ă��Ȃ�
		return false;
	else//���O�C������Ă�
		return true;
}

bool DB::GetIsGroupByID(int talkID) {
	std::string sqlM;
	//�g�[�NID���O���[�v�Ŏg���Ă��邩���擾
	//(MyUserID)
	std::wstring rsql = _TEXT("SELECT COUNT(*) FROM groupi WHERE talk = %d");
	rsql = format(rsql, talkID);
	sqlM = WStringToString(rsql);
	res = stmt->executeQuery(sqlM.c_str());
	res->next();
	bool rtn = res->getInt(1);//����
	ResetResult(res);
	return rtn;
}

void DB::SendChat(int talkID, std::wstring text) {
	std::string sqlM;
	//�ԍ��̎擾
	//(talkID)
	std::wstring rsql1 = _TEXT("SELECT MAX(num) FROM talk WHERE id = %d");
	rsql1 = format(rsql1, talkID);
	sqlM = WStringToString(rsql1);
	res = stmt->executeQuery(sqlM.c_str());
	res->next();
	int maxNum = res->getInt(1) + 1;
	ResetResult(res);
	//���M
	//(talkID,maxNum,MyUserID,text)
	std::wstring sql1 = _TEXT("INSERT INTO talk VALUES(%d,%d,%d,\'%ls\')");
	sql1 = format(sql1, talkID, maxNum, MyUserID, text.c_str());
	sqlM = WStringToString(sql1);
	stmt->execute(sqlM.c_str());
}

Accnt DB::GetGroupByID(int talkID) {
	Accnt act;
	std::string sqlM;
	//�Ή�����O���[�v�����擾����
	//(talkID)
	std::wstring rsql1 = _TEXT("SELECT id,name FROM groupi WHERE talk = %d");
	rsql1 = format(rsql1, talkID);
	sqlM = WStringToString(rsql1);
	res = stmt->executeQuery(sqlM.c_str());
	res->next();
	act.name = StringToWString(res->getString(2).c_str());
	act.number = res->getInt(1);
	ResetResult(res);
	return act;
}

Accnt DB::GetUserByID(int talkID) {
	Accnt act;
	std::string sqlM;
	//�Ή����郆�[�U�[�����擾����
	//(talkID,MyUserID)
	std::wstring rsql1 = (std::wstring)_TEXT("SELECT t1.have,t2.name FROM haveuser t1 INNER JOIN useri t2 ON t1.have = t2.id ") +
							 (std::wstring)_TEXT("WHERE t1.talk = %d AND t1.useri = %d");
	rsql1 = format(rsql1, talkID, MyUserID);
	sqlM = WStringToString(rsql1);
	res = stmt->executeQuery(sqlM.c_str());
	res->next();
	act.name = StringToWString(res->getString(2).c_str());
	act.number = res->getInt(1);
	ResetResult(res);
	return act;
}

AccountS DB::GetUser(void) {
	std::string sqlM;
	//�������ǉ��������[�U�[���擾
	//(MyUserID)
	std::wstring rsql1 = _TEXT("SELECT t1.have,t2.name FROM haveuser t1 INNER JOIN useri t2 ON t1.have = t2.id") +
				(std::wstring)_TEXT(" WHERE t1.useri = %d ");
	rsql1 = format(rsql1,MyUserID);
	sqlM = WStringToString(rsql1);
	res = stmt->executeQuery(sqlM.c_str());
	AccountS acts(res->rowsCount());
	//�擾�E�Z�b�g
	for (int i = 0; i < acts.Count && res->next(); i++) {
		acts[i].name = StringToWString(res->getString(2).c_str());
		acts[i].number = res->getInt(1);
	}
	ResetResult(res);
	return acts;
}

int DB::GetTalkByUser(int userID) {
	std::string sqlM;
	//���[�U�[�Ƃ̃g�[�NID���擾
	//(MyUserID,userID)
	std::wstring rsql1 = _TEXT("SELECT talk FROM haveuser WHERE useri = %d AND have = %d");
	rsql1 = format(rsql1, MyUserID, userID);
	sqlM = WStringToString(rsql1);
	res = stmt->executeQuery(sqlM.c_str());
	res->next();
	int id = res->getInt(1);
	ResetResult(res);
	return id;
}

AccountS DB::GetGroup(void) {
	std::string sqlM;
	//�Q�����Ă�O���[�v�����擾
	//(MyUserID)
	std::wstring rsql1 = _TEXT("SELECT t1.groupi,t2.name FROM gmenber t1 ") +
				(std::wstring)_TEXT("INNER JOIN groupi t2 ON t1.groupi = t2.id  WHERE useri = %d");
	rsql1 = format(rsql1, MyUserID);
	sqlM = WStringToString(rsql1);
	res = stmt->executeQuery(sqlM.c_str());
	AccountS acts(res->rowsCount());
	//�擾�E�Z�b�g
	for (int i = 0; i < acts.Count && res->next(); i++) {
		acts[i].name = StringToWString(res->getString(2).c_str());
		acts[i].number = res->getInt(1);
	}
	ResetResult(res);
	return acts;
}

int DB::GetTalkByGroup(int groupID) {
	std::string sqlM;
	//�O���[�v����g�[�NID���擾
	//(groupID)
	std::wstring rsql1 = _TEXT("SELECT talk FROM groupi WHERE id = %d");
	rsql1 = format(rsql1, groupID);
	sqlM = WStringToString(rsql1);
	res = stmt->executeQuery(sqlM.c_str());
	res->next();
	int id = res->getInt(1);
	ResetResult(res);
	return id;
}

void DB::Load(int talkID) {
	std::string sqlM;
	//�`���b�g�̃��[�h
	//(talkID)
	std::wstring rsql1 = _TEXT("SELECT useri,message,num FROM talk WHERE id = %d ORDER BY num");
	rsql1 = format(rsql1, talkID);
	sqlM = WStringToString(rsql1);
	ChatRes = stmt->executeQuery(sqlM.c_str());
	ChatRes->next();
}

bool DB::GetChatIsMe(void) {
	int id = ChatRes->getInt(1);
	return id == MyUserID;
}

std::wstring DB::GerChatUser(void) {
	int id = ChatRes->getInt(1);
	std::string sqlM;
	//(id)
	std::wstring rsql1 = _TEXT("SELECT name FROM useri WHERE id = %d");
	rsql1 = format(rsql1, id);
	sqlM = WStringToString(rsql1);
	res = stmt->executeQuery(sqlM.c_str());
	res->next();
	std::wstring rtn = StringToWString(res->getString(1).c_str());
	ResetResult(res);
	return rtn;
}

int DB::GetChatCount(void) {
	return ChatRes->rowsCount();
}

std::wstring DB::GetChat(void) {
	return StringToWString(ChatRes->getString(2).c_str());
}

void DB::NextChat(void) {
	ChatRes->next();
}

void DB::ChatEnd(void) {
	ResetResult(ChatRes);
}

void DB::Login(std::wstring pass) {
	try {
		std::wstring sql1 = _TEXT("USE chat");
		std::string sqlM;
		GetMyUeserID_ByFile();
		std::string passM = WStringToString(pass);
		std::string nameM = WStringToString(_TEXT("chat-")) + std::to_string(MyUserID);
		driver = sql::mysql::get_mysql_driver_instance();//�h���C�o�[�̐���
		con = driver->connect(IPhost.c_str(), nameM.c_str(), passM.c_str());//���[�g�A�J�E���g�i�A�J�E���g��o�^���邽�߂̂�j�ɐڑ�
		stmt = con->createStatement();
		sqlM = WStringToString(sql1);
		stmt->execute(sqlM.c_str());//�ړI�̃f�[�^�x�[�X
	}
	catch (sql::SQLException e) {
		//cout << "ERROR!" << endl;
		std::string t1, t2, t3;
		int t4;
		t1 = e.getSQLState();
		t2 = e.getSQLStateCStr();
		t3 = e.what();
		t4 = e.getErrorCode();
	}
}

void DB::MakeAccount(std::wstring name, std::wstring pass) {
	std::wstring comamd;
	comamd = _TEXT("MakeID.exe ") + name + _TEXT(" ") + pass;
	std::string comamdM = WStringToString(comamd);
	int a = system(comamdM.c_str());//�������exe�t�@�C���ɔC����ic++�ō쐬��j
	Login(pass);
}

void DB::AddUser(int id) {
	std::string sqlM;
	int talkID;
	//�g�[�NID�擾
	std::wstring rsql1 = _TEXT("SELECT val FROM variable WHERE name = \'talkid\'");
	sqlM = WStringToString(rsql1);
	res = stmt->executeQuery(sqlM.c_str());
	res->next();
	talkID = res->getInt(1);
	ResetResult(res);
	//�g�[�NID�̍X�V
	std::wstring sql1 = _TEXT("UPDATE variable SET val = val + 1 WHERE name = \'talkid\'");
	sqlM = WStringToString(sql1);
	stmt->execute(sqlM.c_str());
	//���[�U�[�ǉ�
	//(MyUserID,id,talkID)
	std::wstring sql2 = _TEXT("INSERT INTO haveuser VALUES(%d,%d,%d)");
	sql2 = format(sql2, MyUserID, id, talkID);
	sqlM = WStringToString(sql2);
	stmt->execute(sqlM.c_str());
}

void DB::MakeGroup(std::wstring name) {
	std::string sqlM;
	int groupID, talkID;
	//�O���[�vID�̎擾
	std::wstring rsql1 = _TEXT("SELECT val FROM variable WHERE name = \'groupid\'");
	sqlM = WStringToString(rsql1);
	res = stmt->executeQuery(sqlM.c_str());
	res->next();
	groupID = res->getInt(1);
	ResetResult(res);
	//�O���[�vID�̍X�V
	std::wstring sql1 = _TEXT("UPDATE variable SET val = val + 1 WHERE name = \'groupid\'");
	sqlM = WStringToString(sql1);
	stmt->execute(sqlM.c_str());
	//�g�[�NID�̎擾
	std::wstring rsql2 = _TEXT("SELECT val FROM variable WHERE name = \'talkid\'");
	sqlM = WStringToString(rsql2);
	res = stmt->executeQuery(sqlM.c_str());
	res->next();
	talkID = res->getInt(1);
	ResetResult(res);
	//�g�[�NID�̍X�V
	std::wstring sql2 = _TEXT("UPDATE variable SET val = val + 1 WHERE name = \'talkid\'");
	sqlM = WStringToString(sql2);
	stmt->execute(sqlM.c_str());
	//�O���[�v�̍쐬
	//(groupID,name,talkID)
	std::wstring sql3 = _TEXT("INSERT INTO groupi VALUES(%d,\'%ls\',%d)");
	sql3 = format(sql3, groupID, name.c_str(), talkID);
	sqlM = WStringToString(sql3);
	stmt->execute(sqlM.c_str());
	//�O���[�v�̃����o�[�Ƃ��Ď�����o�^
	//(groupID,MyUserID)
	std::wstring sql4 = _TEXT("INSERT INTO gmenber VALUES(%d,%d)");
	sql4 = format(sql4, groupID, MyUserID);
	sqlM = WStringToString(sql4);
	stmt->execute(sqlM.c_str());
}

void DB::AddGroup(int id) {
	std::string sqlM;
	//�O���[�v�����\���҂ɓo�^
	//(id,MyUserID)
	std::wstring sql1 = _TEXT("INSERT INTO gdesire VALUES(%d,%d)");
	sql1 = format(sql1, id, MyUserID);
	sqlM = WStringToString(sql1);
	stmt->execute(sqlM.c_str());
}

AccountS DB::GetDuser(void) {
	std::string sqlM;
	//���[�U�[�o�^�\�������Ă��郆�[�U�[�̎擾
	//(MyUserID,MyUserID))
	std::wstring rsql1 = (std::wstring)_TEXT("SELECT t1.useri,t2.name FROM haveuser t1 ") +
							 (std::wstring)_TEXT("INNER JOIN useri t2 ON t1.useri = t2.id ") +
						 (std::wstring)_TEXT("WHERE t1.have = %d AND t1.useri NOT IN( ") +
							 (std::wstring)_TEXT("SELECT t3.have FROM haveuser t3 WHERE t3.useri=%d)");
	rsql1 = format(rsql1, MyUserID, MyUserID);
	sqlM = WStringToString(rsql1);
	res = stmt->executeQuery(sqlM.c_str());
	AccountS acts = res->rowsCount();
	for (int i = 0; i < acts.Count && res->next(); i++) {
		acts[i].number = res->getInt(1);
		acts[i].name = StringToWString(res->getString(2).c_str());
	}
	ResetResult(res);
	return acts;
}

void DB::AcceptUser(int userID) {
	std::string sqlM;
	int talkID;
	//�Ώۂ̃g�[�NID�̎擾
	//(userID,MyUserID)
	std::wstring rsql1 = _TEXT("SELECT talk FROM haveuser WHERE useri = %d AND have = %d");
	rsql1 = format(rsql1, userID, MyUserID);
	sqlM = WStringToString(rsql1);
	res = stmt->executeQuery(sqlM.c_str());
	res->next();
	talkID = res->getInt(1);
	ResetResult(res);
	//���[�U�[�̒ǉ��i�󂯓���j
	//sql2= format(userID,MyUserID,talkID)
	std::wstring sql1 = _TEXT("INSERT INTO haveuser VALUES(%d,%d,%d)");
	sql1 = format(sql1, MyUserID, userID, talkID);
	sqlM = WStringToString(sql1);
	stmt->execute(sqlM.c_str());
}

AccountS DB::GetDgroup(int groupID) {
	std::string sqlM;
	//�O���[�v�ɉ������悤�Ƃ��Ă郆�[�U�[�̎擾(groupID)
	std::wstring rsql1 = (std::wstring)_TEXT("SELECT t1.useri,t2.name FROM gdesire t1 INNER JOIN useri t2 ON t1.useri = t2.id") +
							 (std::wstring)_TEXT(" WHERE t1.id = %d");
	rsql1 = format(rsql1, groupID);
	sqlM = WStringToString(rsql1);
	res = stmt->executeQuery(sqlM.c_str());
	AccountS acts = res->rowsCount();
	for (int i = 0; i < acts.Count && res->next(); i++) {
		acts[i].name = StringToWString(res->getString(2).c_str());
		acts[i].number = res->getInt(1);
	}
	ResetResult(res);
	return acts;
}

void DB::AcceptGroup(int groupID, int userID) {
	std::string sqlM;
	//�O���[�v�ɒǉ�(groupID,userID)
	std::wstring sql1 = _TEXT("INSERT INTO gmenber VALUES(%d,%d)");
	sql1 = format(sql1, groupID, userID);
	sqlM = WStringToString(sql1);
	stmt->execute(sqlM.c_str());
	//�O���[�v�\���҃��X�g����폜(groupID,userID)
	std::wstring sql2 = _TEXT("DELETE FROM gdesire WHERE id = %d AND useri = %d");
	sql2 = format(sql2, groupID, userID);
	sqlM = WStringToString(sql2);
	stmt->execute(sqlM.c_str());
}
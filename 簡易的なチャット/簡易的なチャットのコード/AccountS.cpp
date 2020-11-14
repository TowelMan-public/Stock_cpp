#include "chat.h"

AccountS::AccountS(int idx) {
	Make(idx);
}

AccountS::AccountS(void) {
	Count = 0;
	m_acnt = nullptr;
}

AccountS::~AccountS() {
	Break();
}

void AccountS::Make(int idx) {
	Count = idx;
	m_acnt = new Accnt[Count];
}

void AccountS::Break(void) {
	Count = 0;
	if (m_acnt != nullptr) delete[] m_acnt;
}

Accnt& AccountS::operator[](int idx) {
	if (m_acnt != nullptr)
		return m_acnt[idx];
	else
		throw((std::string)"�ǂݎ��Ɋւ���A�N�Z�X�G���[");//�ی��̂��߁E�󂯎���Ă��炤�\��͂Ȃ�
}

void AccountS::operator=(AccountS former) {
	Break();
	Make(former.Count);
	//�R�s�[������
	for (int i = 0; i < Count; i++)
		m_acnt[i] = former[i];
}
AccountS::AccountS(const AccountS& m){
	Count = 0;
	m_acnt = nullptr;
	//Break();
	Make(m.Count);
	//�R�s�[������
	for (int i = 0; i < Count; i++)
		m_acnt[i] = (const_cast<AccountS&>(m))[i];
}
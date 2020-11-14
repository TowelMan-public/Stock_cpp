#include "Dentaku.h"

typedef int ERRORNUM;//��O�p�̌^�@Calculation�N���X�ł̂ݗL���Ȍ^

Calculation::Calculation(void) {//������
	IsError = false;
	ErrorNum = NULL;
	m_formula = "";
}

Calculation::~Calculation(void) {//�㏈��
	//����
}

void Calculation::Set(std::string str) {//���̃Z�b�g
	m_formula = str;
}

double Calculation::Do(void) {//�Z�b�g���ꂽ���̌v�Z
	try {
		double ans;
		/*������*/
		m_idx = 0;
		IsError = false;
		Judgment();//���̔���
		/*�v�Z���A���̓������o�͂���*/
		ans = GetAns();
		return ans;
	}
	/*���̊֐��͈̔͂Ŕ����������̂����ʂ�����i�����Ή�����̂͂���̌Ăяo�����j*/
	catch (ERRORNUM ernum) {
		IsError = true;
		ErrorNum = (int)ernum;
		return 0;//�Q�̂Ȃ��l�Ƃ���
	}
}

void Calculation::Judgment(void) {//���̔���
	const int farst = -1;
	const int num = 0;
	const int ten = 4;
	const int enzansi = 1;
	const int kakkos = 2;
	const int kakkof = 3;
	const int zyou = 5;
	int parentheses = 0;
	int befor=farst;
	for (int i = 0; i < m_formula.length(); i++) {
		if (m_formula[i] == '0') {
			if (befor == kakkof) break;
			befor = num;
			continue;
		}
		if (m_formula[i] == '1') {
			if (befor == kakkof) break;
			befor = num;
			continue;
		}
		if (m_formula[i] == '2') {
			if (befor == kakkof) break;
			befor = num;
			continue;
		}
		if (m_formula[i] == '3') {
			if (befor == kakkof) break;
			befor = num;
			continue;
		}
		if (m_formula[i] == '4') {
			if (befor == kakkof) break;
			befor = num;
			continue;
		}
		if (m_formula[i] == '5') {
			if (befor == kakkof) break;
			befor = num;
			continue;
		}
		if (m_formula[i] == '6') {
			if (befor == kakkof) break;
			befor = num;
			continue;
		}
		if (m_formula[i] == '7') {
			if (befor == kakkof) break;
			befor = num;
			continue;
		}
		if (m_formula[i] == '8') {
			if (befor == kakkof) break;
			befor = num;
			continue;
		}
		if (m_formula[i] == '9'){
			if (befor == kakkof) break;
			befor = num;
			continue;
		}
		if (m_formula[i] == '.') {
			if (befor != num) break;
			befor = ten;
			continue;
		}
		if (m_formula[i] == '^') {
			if (befor != num) break;
			befor = zyou;
			continue;
		}
		if (m_formula[i] == '+') {
			if (befor == zyou || befor == enzansi || befor == ten) break;
			befor = enzansi;
			continue;
		}
		if (m_formula[i] == '-') {
			if (befor == zyou || befor == enzansi || befor == ten) break;
			befor = enzansi;
			continue;
		}
		if (m_formula[i] == '*') {
			if (befor == zyou || befor == enzansi || befor == ten) break;
			befor = enzansi;
			continue;
		}
		if (m_formula[i] == '/') {
			if (befor == zyou || befor == enzansi || befor == ten) break;
			befor = enzansi;
			continue;
		}
		if (m_formula[i] == 'r') {
			if (befor == kakkof || befor == ten)break;
			befor = kakkos;
			++i;
			++parentheses;
			continue;
		}
		if (m_formula[i] == 's') {
			if (befor == kakkof || befor == ten)break;
			befor = kakkos;
			++i;
			++parentheses;
			continue;
		}
		if (m_formula[i] == 'c') {
			if (befor == kakkof || befor == ten)break;
			befor = kakkos;
			++i;
			++parentheses;
			continue;
		}
		if (m_formula[i] == 't') {
			if (befor == kakkof || befor == ten)break;
			befor = kakkos;
			++i;
			++parentheses;
			continue;
		}
		if (m_formula[i] == '(') {
			if (befor == kakkof || befor == ten)break;
			befor = kakkos;
			++parentheses;
			continue;
		}
		if (m_formula[i] == ')') {
			if (befor == ten || befor == enzansi) break;
			befor = kakkof;
			if (--parentheses >= 0)//���̐��ɂȂ������_�ŕs��
				continue;
		}
		throw((ERRORNUM)CLCT_ERROR_FORMULA);//�������s��
	}
	if (parentheses != 0)throw((ERRORNUM)CLCT_ERROR_FORMULA);//���̎��_�ł������̐���0�łȂ���Εs��
	if (befor != num && befor != kakkof)throw((ERRORNUM)CLCT_ERROR_FORMULA);
}

double  Calculation::GetAns(void) {//���̌v�Z�̎��s�i�����m�𑫂������邱�Ƃɂ��l�����߂�j
	double ans = 0;
	for (; m_idx < m_formula.length(); /*m_idx++*/) {
		ans += GetArgumentValue();
		if (m_idx < m_formula.length()&&m_formula[m_idx] == ')') {//�������̕������������̎��̌v�Z�͏I��
			++m_idx;
			break;
		}
	}
	return ans;
}

double Calculation::GetArgumentValue(void) {//���̒l�����߂�
	double seki = 1, syou = 1;
	if (m_formula[m_idx] == '-')seki = -1;
	if (m_formula[m_idx] == '+' || m_formula[m_idx] == '-')++m_idx;
	for (; m_idx < m_formula.length(); /*m_idx++*/) {
		if (m_formula[m_idx] == '+' || m_formula[m_idx] == '-' || m_formula[m_idx] == ')') break;
		if (m_formula[m_idx] == '*')++m_idx;
		if (m_formula[m_idx] != '/') seki *= GetValue();
		else ++m_idx, syou *= GetValue();
	}
	if (syou == 0)throw((ERRORNUM)CLCT_ERROR_WARI);
	return seki / syou;
}

double Calculation::GetValue(void) {
	double temp = 0;
	std::string strnum = "";
	if (m_formula[m_idx] == 's') {
		m_idx += 2;
		return sin(GetAns());
	}
	if (m_formula[m_idx] == 'c') {
		m_idx += 2;
		return cos(GetAns());
	}
	if (m_formula[m_idx] == 't') {
		m_idx += 2;
		return tan(GetAns());
	}
	if (m_formula[m_idx] == '(') {
		++m_idx;
		return GetAns();
	}
	if (m_formula[m_idx] == 'r') {
		m_idx += 2;
		temp = GetAns();
		if (temp < 0)throw((ERRORNUM)CLCT_ERROR_ROOT);
		return sqrt(temp);
	}
	/*�l�����߂�i�u�l^�l�v���l�ł���j*/
	for (int i = 1; m_idx < m_formula.length(); (i == 1 ? 1 : i - 1), m_idx++) {
		if (atoi((m_formula.substr(m_idx,1).c_str())) == 0 && m_formula[m_idx] != '0' && m_formula[m_idx] != '.')break;
		strnum += m_formula[m_idx];
	}
	temp = std::stod(strnum.c_str());
	if(m_formula[m_idx]!='^')return temp;
	else {
		++m_idx;
		return pow(temp, GetAns());
	}
}
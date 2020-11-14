#include "Dentaku.h"

typedef int ERRORNUM;//例外用の型　Calculationクラスでのみ有効な型

Calculation::Calculation(void) {//初期化
	IsError = false;
	ErrorNum = NULL;
	m_formula = "";
}

Calculation::~Calculation(void) {//後処理
	//未定
}

void Calculation::Set(std::string str) {//式のセット
	m_formula = str;
}

double Calculation::Do(void) {//セットされた式の計算
	try {
		double ans;
		/*初期化*/
		m_idx = 0;
		IsError = false;
		Judgment();//式の判定
		/*計算し、その答えを出力する*/
		ans = GetAns();
		return ans;
	}
	/*この関数の範囲で発生したものを識別させる（これを対応するのはこれの呼び出し元）*/
	catch (ERRORNUM ernum) {
		IsError = true;
		ErrorNum = (int)ernum;
		return 0;//害のない値として
	}
}

void Calculation::Judgment(void) {//式の判定
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
			if (--parentheses >= 0)//負の数になった時点で不正
				continue;
		}
		throw((ERRORNUM)CLCT_ERROR_FORMULA);//何かが不正
	}
	if (parentheses != 0)throw((ERRORNUM)CLCT_ERROR_FORMULA);//この時点でかっこの数が0でなければ不正
	if (befor != num && befor != kakkof)throw((ERRORNUM)CLCT_ERROR_FORMULA);
}

double  Calculation::GetAns(void) {//式の計算の実行（項同士を足し続けることにより値を求める）
	double ans = 0;
	for (; m_idx < m_formula.length(); /*m_idx++*/) {
		ans += GetArgumentValue();
		if (m_idx < m_formula.length()&&m_formula[m_idx] == ')') {//かっこの閉じるやつが来たら一つの式の計算は終了
			++m_idx;
			break;
		}
	}
	return ans;
}

double Calculation::GetArgumentValue(void) {//項の値を求める
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
	/*値を求める（「値^値」も値である）*/
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
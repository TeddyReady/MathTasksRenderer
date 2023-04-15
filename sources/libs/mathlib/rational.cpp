#include "rational.h"

Rational::Rational() : num(0), den(1){}
Rational::Rational(int num, int den) : num(num), den(den){
    if (den < 0) {this->den = -den; this->num = -num;}
    else if (den == 0) {this->den = 1;}
}

Rational Rational::simplify(){
    int NOD = GCD(this->den, this->num);
    this->den /= NOD;
    this->num /= NOD;
    if (this->num == 0) {this->den = 1;}
    return *this;
}

Rational Rational::operator-() const {
    Rational ratio;
    ratio.num = -this->num;
    ratio.den = this->den;
    return ratio;
}
Rational Rational::operator+(Rational ratio) const {
    Rational summary;
    if (this->den == ratio.den){
        summary.den = ratio.den;
        summary.num = this->num + ratio.num;
    } else {
        summary.den = ratio.den * this->den;
        summary.num = (this->num * ratio.den) + (this->den * ratio.num);
    }
    return summary.simplify();
}
Rational Rational::operator+=(Rational ratio){
    Rational summary;
    summary = *(this) + ratio;
    *this = summary;
    return *this;
}
Rational Rational::operator-(Rational ratio) const {
    Rational summary;
    if (this->den == ratio.den){
        summary.den = ratio.den;
        summary.num = this->num - ratio.num;
    } else {
        summary.den = ratio.den * this->den;
        summary.num = (this->num * ratio.den) - (this->den * ratio.num);
    }
    return summary.simplify();
}
Rational Rational::operator-=(Rational ratio){
    Rational summary;
    summary = *(this) - ratio;
    *this = summary;
    return *this;
}
Rational Rational::operator*(Rational ratio) const {
    Rational summary;
    summary.den = this->den * ratio.den;
    summary.num = this->num * ratio.num;

    return summary.simplify();
}
Rational Rational::operator*=(Rational ratio){
    Rational summary;
    summary = *(this) * ratio;
    *this = summary;
    return *this;
}
Rational Rational::operator/(Rational ratio) const {
    Rational summary;
    if(ratio.num != 0){
        summary.den = this->den * ratio.num;
        summary.num = this->num * ratio.den;

        return summary.simplify();
    }
    else{
        std::cerr << "DETECTED ERROR: Divide by Zero!" << std::endl;
        return *this;
    }
}
Rational Rational::operator/=(Rational ratio){
    Rational summary;
    summary = *(this) / ratio;
    *this = summary;
    return *this;
}

bool Rational::operator==(Rational ratio) const {
    Rational tmp = *this;
    tmp = tmp.simplify();
    ratio = ratio.simplify();
    if((ratio.den == tmp.den) && (ratio.num == tmp.num)){
        return true;
    }
    return false;
}
bool Rational::operator!=(Rational ratio) const {
    Rational tmp = *this;
    tmp = tmp.simplify();
    ratio = ratio.simplify();
    if((ratio.den != tmp.den) || (ratio.num != tmp.num)){
        return true;
    }
    return false;
}
bool Rational::operator<(Rational ratio) const {
    Rational tmp = *this;
    int tmp_den = tmp.den;

    tmp.num *= ratio.den;
    ratio.num *= tmp.den;

    tmp.den *= ratio.den;
    ratio.den *= tmp_den; 

    tmp = tmp.simplify();
    ratio = ratio.simplify();
    if(tmp.num < ratio.num){return true;}
    return false;
}
bool Rational::operator<=(Rational ratio) const {
    Rational tmp = *this;
    int tmp_den = tmp.den;

    tmp.num *= ratio.den;
    ratio.num *= tmp.den;

    tmp.den *= ratio.den;
    ratio.den *= tmp_den; 

    tmp = tmp.simplify();
    ratio = ratio.simplify();
    if(tmp.num <= ratio.num){return true;}
    return false;
}
bool Rational::operator>(Rational ratio) const {
    Rational tmp = *this;
    int tmp_den = tmp.den;

    tmp.num *= ratio.den;
    ratio.num *= tmp.den;

    tmp.den *= ratio.den;
    ratio.den *= tmp_den; 

    tmp = tmp.simplify();
    ratio = ratio.simplify();
    if(tmp.num > ratio.num){return true;}
    return false;
}
bool Rational::operator>=(Rational ratio) const {
    Rational tmp = *this;
    int tmp_den = tmp.den;

    tmp.num *= ratio.den;
    ratio.num *= tmp.den;

    tmp.den *= ratio.den;
    ratio.den *= tmp_den; 

    tmp = tmp.simplify();
    ratio = ratio.simplify();
    if(tmp.num >= ratio.num){return true;}
    return false;
}

Rational::operator int() const {
    return (int)this->num/this->den;
}
Rational::operator double()const {
    return (double)this->num/this->den;
}

std::ostream& operator<<(std::ostream& out, const Rational& ratio){
    return out << ratio.num << "/" << ratio.den << std::endl;
}

std::istream& operator>>(std::istream& in, Rational& ratio){
    std::cout << "Input numerator: "; 
    in >> ratio.num;
    do{
        std::cout << std::endl << "Input denumerator: ";
        in >> ratio.den;
        if(!ratio.den){std::cerr << "Denumerator cannot be Zero" << std::endl;}
    } while (!ratio.den);
    if (ratio.num == 0) { ratio.den = 0;}
    if (ratio.den < 0) { ratio.num = -ratio.num; ratio.den = -ratio.den;}
    return in;
}

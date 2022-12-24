//
// Created by Christoph Rohde on 24.12.22.
//

#pragma once

#include <iostream>
#include <../StringUtils.hpp>

class CronPart {
private:
    std::string name;
    std::string rawValue;

    bool isNumber;
    bool isRange;
    bool isList;
    bool isWildcard;
    bool isPeriodic;

    int value;
    int min;
    int max;
    int step;


// 1-31 || * || */2 || 1,2,3 || 1-5 || 1-5/2 || 1-5,7,9  String
    void processValue(const std::string &basicString) {
        this-> isNumber = StringUtils::is_number(basicString);
        if ( isNumber){
            // 1
            // basicString is value
        }


//        auto components = StringUtils::split_by_any_of(basicString, "-");


        this->isWildcard = StringUtils::contains(basicString, "*");
        if(this->isWildcard && basicString.length() == 1){
            // *
            // basicString is wildcard
        }else if(this->isWildcard && basicString.length() > 1){
            // */2
            // basicString is periodic
        }

        this->isRange = StringUtils::contains(basicString, "-");
        if(this->isRange){
            // 1-5
            // basicString is range
            std::vector<std::string> range = StringUtils::split_by(basicString, '-');
        }


        this->isList = StringUtils::contains(basicString, ",");
        if(this->isList){
            // 1,2,3
            // basicString is list
            std::vector<std::string> list = StringUtils::split_by(basicString, ',');

        }

        this->isPeriodic = StringUtils::contains(basicString, "/");
        if(this->isPeriodic){
            // 1-5/2
            // basicString is periodic
        }









    }

public:



    CronPart(const std::string& name, const std::string &rawValue)
        : name(name), rawValue(rawValue){

        processValue(rawValue);
    }

    ~CronPart() = default;




};

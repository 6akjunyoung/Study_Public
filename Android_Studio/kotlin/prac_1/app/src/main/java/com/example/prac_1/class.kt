package com.example.prac_1

fun main() {
    // class란 유사 기능(함수)의 집합체(객체)

    // 1. class - 자동차(시동, 운전), 사람(밥먹는다, 걷는다, 물마신다)
    // 2. data class - set, get

    // 1. 코딩
    // 2. 호출

    var cls = HelloClass()
    var cls2 = HelloClass(1)
    println(cls2.age)

    var person = Person(26, "Jun")
    println("$person")
}

class HelloClass {
    var age: Int = 0
    init {

    }

    // default 생성자, 보조 생성자
    constructor()           // 기본 생성자 (값을 못 넘기지만 초기화 가능)
    constructor(age: Int) { // 보조 생성자 (값을 넘길 수 있고, 초기화 가능)
        this.age = age
    }
}

data class Person(var age: Int, val name: String)
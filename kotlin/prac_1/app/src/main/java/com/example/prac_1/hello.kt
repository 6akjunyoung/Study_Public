package com.example.prac_1

fun main() {
    val hello1: String = "hello world 1"        // final
    var hello2: String = "hello world 2"        // int, string, float, double
                                                //hello2 = null 불가
    var hello3: String? = null                  // null

    println(hello1)
    println(hello3)

    println(hello())
}

fun hello(): String {   // void     // return String
    return "hello fun"
}
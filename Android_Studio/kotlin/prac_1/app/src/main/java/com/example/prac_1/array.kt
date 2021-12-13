package com.example.prac_1

fun main() {
    // 1. 배열
    var arr1 = listOf("1","2")              // 수정 불가
    var arr2 = mutableListOf("1","2")       // 수정 가능
                                            // arr1.add("3") 불가
    arr2.add("3")

    // 2. loop
    for(item in arr1) {
        println(item)
    }

    for((index, item) in arr1.withIndex()) {
        println("$index, $item")
    }

    // 3. casting object
    var hello: Any = "hello"
    if(hello is String) {
        var str: String = hello
    }
    println(hello)
}
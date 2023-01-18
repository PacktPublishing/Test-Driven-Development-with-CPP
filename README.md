


# Test Driven Development with C++

<a href="https://www.amazon.com/Test-Driven-Development-Writing-Bug-free/dp/1803242000/ref=tmm_pap_swatch_0?_encoding=UTF8&qid=&sr=&utm_source=github&utm_medium=repository&utm_campaign=9781804613900"><img src="https://m.media-amazon.com/images/I/41RyuU0nPbL._SX404_BO1,204,203,200_.jpg" alt="Test Driven Development with C++" height="256px" align="right"></a>

This is the code repository for [Test Driven Development with C++](https://www.amazon.com/Test-Driven-Development-Writing-Bug-free/dp/1803242000/ref=tmm_pap_swatch_0?_encoding=UTF8&qid=&sr=&utm_source=github&utm_medium=repository&utm_campaign=9781804613900), published by Packt.

**A simple guide to writing bug-free Agile code**

## What is this book about?
Modern, standard C++ is all that is needed to create a small and practical testing framework that will improve the design of any project. This allows you to think about how the code will be used, which is the first step in designing intuitive interfaces. TDD is a modern balanced software development approach that would help create maintainable applications, provide modularity in design and write minimal code that would drastically reduce defects. With the help of this book, you’ll be able to continue adding value when designs need to change by ensuring that the changes don’t break existing tests.

This book covers the following exciting features:
* Understand how to develop software using TDD
* Keep the code for the system as error-free as possible
* Refactor and redesign code confidently
* Communicate the requirements and behaviors of the code with your team
* Understand the differences between unit tests and integration tests
* Use TDD to create a minimal viable testing framework

If you feel this book is for you, get your [copy](https://www.amazon.com/dp/1803242000) today!

<a href="https://www.packtpub.com/?utm_source=github&utm_medium=banner&utm_campaign=GitHubBanner"><img src="https://raw.githubusercontent.com/PacktPublishing/GitHub/master/GitHub.png" 
alt="https://www.packtpub.com/" border="5" /></a>

## Instructions and Navigations
All of the code is organized into folders. For example, Chapter02.

The code will look like the following:
```
TEST("Test bool confirms")
{
    bool result = isNegative(0);
    CONFIRM_FALSE(result);
    result = isNegative(-1);
    CONFIRM_TRUE(result);
}
```

**Following is what you need for this book:**
This book is for C++ developers already familiar with and using C++ for daily tasks who want to improve their skillset. You don’t need to be an expert but you should already have some knowledge of modern C++ and how to use templates to get the most out of this book.

With the following software and hardware list you can run all code files present in the book (Chapter 1-15).
### Software and Hardware List
| Chapter | Software required | OS required |
| -------- | ------------------------------------ | ----------------------------------- |
| 1-15 | Any C++ compiler, minimally C++20 | Windows, Mac OS X, and Linux (Any) |


### Related products
* C++20 STL Cookbook [[Packt]](https://www.packtpub.com/product/c20-stl-cookbook/9781803248714?utm_source=github&utm_medium=repository&utm_campaign=9781803248714) [[Amazon]](https://www.amazon.com/dp/1803248718)

* Template Metaprogramming with C++ [[Packt]](https://www.packtpub.com/product/template-metaprogramming-with-c/9781803243450?utm_source=github&utm_medium=repository&utm_campaign=9781803243450) [[Amazon]](https://www.amazon.com/dp/1803243457)


## Get to Know the Author
**Abdul Wahid Tanner**
is the founder and an instructor at Take Up Code which teaches programmers the joys of coding through live classes, podcasts, books and more. He has authored two self-published titles, "How To Code C++ From A Simple Idea To A Game You’ll Understand!" and "The C++ Game Starter Template System". He was previously working as a software engineer at Bloomberg LP, and a Principal Software Engineer at Citrix. With over four decades of experience in the tech world, he has worked with established companies such as Dell and Microsoft in the past, and has been a speaker at CppCon. He currently resides in Texas, USA.


### Download a free PDF

 <i>If you have already purchased a print or Kindle version of this book, you can get a DRM-free PDF version at no cost.<br>Simply click on the link to claim your free PDF.</i>
<p align="center"> <a href="https://packt.link/free-ebook/9781803242002">https://packt.link/free-ebook/9781803242002 </a> </p>
Recommender System Assignment
==================

C++ Implementation of recommender system with collaborative filtering.

To run the implementation
=========================

1. Keep project files in one folder.

2. compile using command `make`.
> To compile without using the makefile, type the following command.
>
> `g++ -std=c++11 recommender.cpp -o recommender.exe`
> 
> (Note that -std=c++11 option is must be given in g++.)

3. Run using following command.
> ./recommender.exe [base file name] [test file name]


Summary of the algorithm
============

Collaborative filtering (CF) algorithm is the most prominent approach to generate recommendations.

**Advantages**
1. used by large, commercial e-commerce sites
2. well-understood, various algorithms and variations exist
3. applicable in many domains (book, movies, DVDs, ..)

**Basic assumption and idea**
1. Users give ratings to catalog items (implicitly or explicitly)
2. Customers who had similar tastes in the past, will have similar tastes in the future

**Approach**
1. Consider user c
2. Find set D of other users whose ratings are “similar” to c’s ratings
3. Estimate user’s ratings based on ratings of users in D

**Input**
1. A matrix of given user–item ratings

**Output**
1. A (numerical) prediction indicating to what degree the current user will like or dislike a certain item. 
2. A top-N list of recommended items.

**Method**

There are two types of collaborative filtering.
1. User-based
2. Item-based


Any other specification of the implementation and testing
============

- Note that I use c++11, not c++. therefore -std=c++11 option is must be given in g++.

- `self test result`

u1 RSME: 0.9094779

u2 RSME: 0.8995277

u3 RSME: 0.8900281

u4 RSME: 0.8941476

u5 RSME: 0.8968278


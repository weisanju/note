# Advisor

Base interface holding AOP advice (action to take at a joinpoint) and a filter determining the applicability of the advice (such as a pointcut). 

包含 AOP advice 和确定 advice的 applicability  的 过滤器（例如 *pointcut*）的 基本接口



This interface is not for use by Spring users, but to allow for commonality in support for different types of advice.
Spring AOP is based around around advice delivered via method interception, compliant with the AOP Alliance interception API. The Advisor interface allows support for different types of advice, such as before and after advice, which need not be implemented using interception.
Author:
Rod Johnson, Juergen Hoeller


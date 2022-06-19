# Expect

在编写测试时，通常需要检查值是否满足某些条件。expect使您可以访问许多 “matchers”，使您可以验证不同的内容。



## expect(value)

1. 每次要测试值时都会使用expect函数。很少 expect  值 本身
2. 相反，您将使用expect和 “matcher” 函数来断言某个值。

3. 用一个例子来理解这一点更容易。假设你有一个方法bestroixflavor () 应该返回字符串 'grapefruit'。以下是你将如何测试:



```typescript
test('the best flavor is grapefruit', () => {
  expect(bestLaCroixFlavor()).toBe('grapefruit');
});
```

在这种情况下，toBe是matcher function。下面记录了许多不同的匹配器功能，以帮助您测试不同的东西。
期望的参数应该是您的代码产生的值，并且匹配器的任何参数都应该是正确的值。





## expect.extend(matchers)

使用*expect.extend* 扩展自己的matchers到Jest。

例如，假设您正在测试数字实用程序库，并且经常断言数字出现在其他数字的特定范围内。你可以把它抽象成一个toBeWithinRange匹配器:

```typescript
expect.extend({
  toBeWithinRange(received, floor, ceiling) {
    const pass = received >= floor && received <= ceiling;
    if (pass) {
      return {
        message: () =>
          `expected ${received} not to be within range ${floor} - ${ceiling}`,
        pass: true,
      };
    } else {
      return {
        message: () =>
          `expected ${received} to be within range ${floor} - ${ceiling}`,
        pass: false,
      };
    }
  },
});

test('numeric ranges', () => {
  expect(100).toBeWithinRange(90, 110);
  expect(101).not.toBeWithinRange(0, 100);
  expect({apples: 6, bananas: 3}).toEqual({
    apples: expect.toBeWithinRange(1, 10),
    bananas: expect.not.toBeWithinRange(11, 20),
  });
});
```

注意: 在TypeScript中，例如使用 @ types/jest时，您可以在导入的模块中声明新的toBeWithinRange匹配器，如下所示:

```typescript
interface CustomMatchers<R = unknown> {
  toBeWithinRange(floor: number, ceiling: number): R;
}

declare global {
  namespace jest {
    interface Expect extends CustomMatchers {}
    interface Matchers<R> extends CustomMatchers<R> {}
    interface InverseAsymmetricMatchers extends CustomMatchers {}
  }
}
```



## expect.anything()

`expect.anything()` 除了 null或者 undefine其他都匹配

 You can use it inside `toEqual` or `toBeCalledWith` instead of a literal value. 

一般在  `toEqual` or `toBeCalledWith` 内部使用 

```typescript
test('map calls its argument with a non-null argument', () => {
  const mock = jest.fn();
  [1].map(x => mock(x));
  expect(mock).toBeCalledWith(expect.anything());
});
```

## expect.any(constructor)

`expect.any(constructor)` matches anything that was created with the given constructor or if it's a primitive that is of the passed type. You can use it inside `toEqual` or `toBeCalledWith` instead of a literal value. For example, if you want to check that a mock function is called with a number:

```python
class Cat {}
function getCat(fn) {
  return fn(new Cat());
}

test('randocall calls its callback with a class instance', () => {
  const mock = jest.fn();
  getCat(mock);
  expect(mock).toBeCalledWith(expect.any(Cat));
});

function randocall(fn) {
  return fn(Math.floor(Math.random() * 6 + 1));
}

test('randocall calls its callback with a number', () => {
  const mock = jest.fn();
  randocall(mock);
  expect(mock).toBeCalledWith(expect.any(Number));
});
```



## expect.arrayContaining(array)

`expect.arrayContaining(array)` 匹配数组元素

即 期望的数组必须是 返回数组的子集 

```typescript
describe('arrayContaining', () => {
  const expected = ['Alice', 'Bob'];
  it('matches even if received contains additional elements', () => {
    expect(['Alice', 'Bob', 'Eve']).toEqual(expect.arrayContaining(expected));
  });
  it('does not match if received does not contain expected elements', () => {
    expect(['Bob', 'Eve']).not.toEqual(expect.arrayContaining(expected));
  });
});
```

```typescript
describe('Beware of a misunderstanding! A sequence of dice rolls', () => {
  const expected = [1, 2, 3, 4, 5, 6];
  it('matches even with an unexpected number 7', () => {
    expect([4, 1, 6, 7, 3, 5, 2, 5, 4, 6]).toEqual(
      expect.arrayContaining(expected),
    );
  });
  it('does not match without an expected number 2', () => {
    expect([4, 1, 6, 7, 3, 5, 7, 5, 4, 6]).not.toEqual(
      expect.arrayContaining(expected),
    );
  });
});
```

## expect.assertions(number)

`expect.assertions(number)` 

验证在测试过程中调用了一定数量的断言，这在测试异步代码很有用，以确保回调中的断言实际上被调用了

例如， 有两个 异步 函数 callback1 callback2，它将以未知顺序异步调用它们。我们可以用:

```js
test('doAsync calls both callbacks', () => {
  expect.assertions(2);
  function callback1(data) {
    expect(data).toBeTruthy();
  }
  function callback2(data) {
    expect(data).toBeTruthy();
  }

  doAsync(callback1, callback2);
});
```

The `expect.assertions(2)` call ensures that both callbacks actually get called.


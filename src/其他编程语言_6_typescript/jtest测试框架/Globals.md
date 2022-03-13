# Globals

在您的测试文件中，Jest将这些方法和对象中的每一个都放入全局环境中。您不必要求或导入任何东西即可使用它们。但是，如果您更喜欢显式导入，则可以从 “@ jest/globals” 中  	`import {describe, expect, test} from '@jest/globals'`。

## [`afterAll(fn, timeout)`](https://jestjs.io/docs/api#afterallfn-timeout)

* 在此文件中的所有测试完成后运行一个函数。如果函数返回 `Promise` 或是`generator`，则Jest等待该承诺解决后再继续。

* 您可以提供一个超时时间 (以毫秒为单位)，用于指定在中止之前等待多长时间。注意: 默认超时时间为5秒。
  如果要清理跨测试共享的某些全局设置状态，这通常很有用。

```typescript
const globalDatabase = makeGlobalDatabase();

function cleanUpDatabase(db) {
  db.cleanUp();
}

afterAll(() => {
  cleanUpDatabase(globalDatabase);
});

test('can find things', () => {
  return globalDatabase.find('thing', {}, results => {
    expect(results.length).toBeGreaterThan(0);
  });
});

test('can insert a thing', () => {
  return globalDatabase.insert('thing', makeThing(), response => {
    expect(response.success).toBeTruthy();
  });
});
```

## [`afterEach(fn, timeout)`](https://jestjs.io/docs/api#aftereachfn-timeout)

当每一个test结束时 执行的动作

```typescript
const globalDatabase = makeGlobalDatabase();

function cleanUpDatabase(db) {
  db.cleanUp();
}

afterEach(() => {
  cleanUpDatabase(globalDatabase);
});

test('can find things', () => {
  return globalDatabase.find('thing', {}, results => {
    expect(results.length).toBeGreaterThan(0);
  });
});

test('can insert a thing', () => {
  return globalDatabase.insert('thing', makeThing(), response => {
    expect(response.success).toBeTruthy();
  });
});
```



## [`beforeAll(fn, timeout)`](https://jestjs.io/docs/api#beforeallfn-timeout)

所有test文件执行前执行

```typescript
const globalDatabase = makeGlobalDatabase();

beforeAll(() => {
  // Clears the database and adds some testing data.
  // Jest will wait for this promise to resolve before running tests.
  return globalDatabase.clear().then(() => {
    return globalDatabase.insert({testData: 'foo'});
  });
});

// Since we only set up the database once in this example, it's important
// that our tests don't modify it.
test('can find things', () => {
  return globalDatabase.find('thing', {}, results => {
    expect(results.length).toBeGreaterThan(0);
  });
});
```



## [`beforeEach(fn, timeout)`](https://jestjs.io/docs/api#beforeeachfn-timeout)

在每个测试任务运行前执行

```typescript
const globalDatabase = makeGlobalDatabase();

beforeEach(() => {
  // Clears the database and adds some testing data.
  // Jest will wait for this promise to resolve before running tests.
  return globalDatabase.clear().then(() => {
    return globalDatabase.insert({testData: 'foo'});
  });
});

test('can find things', () => {
  return globalDatabase.find('thing', {}, results => {
    expect(results.length).toBeGreaterThan(0);
  });
});

test('can insert a thing', () => {
  return globalDatabase.insert('thing', makeThing(), response => {
    expect(response.success).toBeTruthy();
  });
});
```

## [`describe(name, fn)`](https://jestjs.io/docs/api#describename-fn)

创建一个块，该块将几个相关测试分组在一起。

```python
const myBeverage = {
  delicious: true,
  sour: false,
};

describe('my beverage', () => {
  test('is delicious', () => {
    expect(myBeverage.delicious).toBeTruthy();
  });

  test('is not sour', () => {
    expect(myBeverage.sour).toBeFalsy();
  });
});
```

这不是必需的-您可以直接在顶层编写测试块。但是，如果您希望将测试分为组，这可能会很方便。
如果你有测试的层次结构，你也可以嵌套描述块:

```typescript
const binaryStringToNumber = binString => {
  if (!/^[01]+$/.test(binString)) {
    throw new CustomError('Not a binary number.');
  }

  return parseInt(binString, 2);
};

describe('binaryStringToNumber', () => {
  describe('given an invalid binary string', () => {
    test('composed of non-numbers throws CustomError', () => {
      expect(() => binaryStringToNumber('abc')).toThrowError(CustomError);
    });

    test('with extra whitespace throws CustomError', () => {
      expect(() => binaryStringToNumber('  100')).toThrowError(CustomError);
    });
  });

  describe('given a valid binary string', () => {
    test('returns the correct number', () => {
      expect(binaryStringToNumber('100')).toBe(4);
    });
  });
});
```



## [`describe.each(table)(name, fn, timeout)`](https://jestjs.io/docs/api#describeeachtablename-fn-timeout)

* 使用不同的数据执行重复的测试集
* name: 测试套件的名称

* `describe.each(table)(name, fn, timeout)`

* 通过占位符注入参数生成唯一的测试标题

  `printf` formatting

  - `%p` - [pretty-format](https://www.npmjs.com/package/pretty-format).
  - `%s`- String.
  - `%d`- Number.
  - `%i` - Integer.
  - `%f` - Floating point value.
  - `%j` - JSON.
  - `%o` - Object.
  - `%#` - Index of the test case.
  - `%%` - single percent sign ('%'). This does not consume an argument.

* 或通过使用 $variable注入测试用例对象的属性来生成唯一的测试标题

  - 嵌套对象使用： `$variable.path.to.value`

  - 可以使用 `$#`注入测试用例的索引

  - 您不能将 “$ variableable” 与 “printf” format 一起使用，除非 使用 %%

* fn: 函数要运行的测试套件，这是将接收每行中的参数作为函数参数的函数。

* 您可以提供一个超时时间 (以毫秒为单位)，用于指定在中止之前等待每一行多长时间。注意: 默认超时时间为5秒。

```typescript
describe.each([
  [1, 1, 2],
  [1, 2, 3],
  [2, 1, 3],
])('.add(%i, %i)', (a, b, expected) => {
  test(`returns ${expected}`, () => {
    expect(a + b).toBe(expected);
  });

  test(`returned value not be greater than ${expected}`, () => {
    expect(a + b).not.toBeGreaterThan(expected);
  });

  test(`returned value not be less than ${expected}`, () => {
    expect(a + b).not.toBeLessThan(expected);
  });
});
```

```typescript
describe.each([
  {a: 1, b: 1, expected: 2},
  {a: 1, b: 2, expected: 3},
  {a: 2, b: 1, expected: 3},
])('.add($a, $b)', ({a, b, expected}) => {
  test(`returns ${expected}`, () => {
    expect(a + b).toBe(expected);
  });

  test(`returned value not be greater than ${expected}`, () => {
    expect(a + b).not.toBeGreaterThan(expected);
  });

  test(`returned value not be less than ${expected}`, () => {
    expect(a + b).not.toBeLessThan(expected);
  });
});
```

### describe.each\`table\`(name, fn, timeout)

**table**

标记模板文字

**name**

测试套件名称

```python
describe.each`
  a    | b    | expected
  ${1} | ${1} | ${2}
  ${1} | ${2} | ${3}
  ${2} | ${1} | ${3}
`('$a + $b', ({a, b, expected}) => {
  test(`returns ${expected}`, () => {
    expect(a + b).toBe(expected);
  });

  test(`returned value not be greater than ${expected}`, () => {
    expect(a + b).not.toBeGreaterThan(expected);
  });

  test(`returned value not be less than ${expected}`, () => {
    expect(a + b).not.toBeLessThan(expected);
  });
});
```

### [`describe.only(name, fn)`](https://jestjs.io/docs/api#describeonlyname-fn)

同名函数：`fdescribe(name, fn)`

只运行该测试块，而不运行其他测试块

* `describe.only.each(table)(name, fn)`
* `describe.only.each`table`(name, fn)`

```typescript
describe.only('my beverage', () => {
  test('is delicious', () => {
    expect(myBeverage.delicious).toBeTruthy();
  });

  test('is not sour', () => {
    expect(myBeverage.sour).toBeFalsy();
  });
});

describe('my other beverage', () => {
  // ... will be skipped
});
```

## `describe.skip(name, fn)`

* 跳过该测试块
* 同义函数：`xdescribe(name, fn)`
* 使用`describe.skip` 一般用来临时取消掉不用的注释。请注意，describe块仍将运行。如果您有一些也应该跳过的设置，请在beforall或beforeach块中进行。
* `describe.skip.each(table)(name, fn)`
* xdescribe.\`each`table(name, fn)
* describe.skip.each\`table`(name, fn)

```typescript
describe('my beverage', () => {
  test('is delicious', () => {
    expect(myBeverage.delicious).toBeTruthy();
  });

  test('is not sour', () => {
    expect(myBeverage.sour).toBeFalsy();
  });
});

describe.skip('my other beverage', () => {
  // ... will be skipped
});
```

## `test(name, fn, timeout)`

* 同义函数：`it(name, fn, timeout)`

```typescript
test('did not rain', () => {
  expect(inchesOfRain()).toBe(0);
});
```

* 第一个参数是测试名称

* 第二个参数是包含要测试的期望的函数。
* 第三个参数 (可选) 是超时 (以毫秒为单位)，用于指定中止前等待多长时间。注意: 默认超时时间为5秒。

* 如果从test返回promise ，Jest将等待Promis resloved，然后再完成测试。

* 如果您为测试函数提供了一个参数，名字叫做，done，那么Jest也会等待。

当你想测试回调时，这可能很方便。[here](https://jestjs.io/docs/asynchronous#callbacks)  看到如何测试异步代码。

例如，假设fetchBeverageList() 返回一个承诺，该承诺应该解析为包含柠檬的列表。您可以使用以下方法进行测试:

```typescript
test('has lemon in it', () => {
  return fetchBeverageList().then(list => {
    expect(list).toContain('lemon');
  });
});
```

即使测试调用将立即返回，测试也要等到promise解决后才能完成。

### [`test.concurrent(name, fn, timeout)`](https://jestjs.io/docs/api#testconcurrentname-fn-timeout)

* `it.concurrent(name, fn, timeout)`
* `test.concurrent.each(table)(name, fn, timeout)`
* `test.concurrent.only.each(table)(name, fn)`
* `test.concurrent.skip.each(table)(name, fn)`
* `test.concurrent.skip.each`table`(name, fn)`

* 第一个参数是测试名称; 
* 第二个参数是包含要测试的期望的异步函数。
* 第三个参数 (可选) 是超时 (以毫秒为单位)，用于指定中止前等待多长时间。注意: 默认超时时间为5秒。

```typescript
test.concurrent('addition of 2 numbers', async () => {
  expect(5 + 3).toBe(8);
});

test.concurrent('subtraction 2 numbers', async () => {
  expect(5 - 3).toBe(2);
});
```

## `test.each(table)(name, fn, timeout)`

## `test.only(name, fn, timeout)`
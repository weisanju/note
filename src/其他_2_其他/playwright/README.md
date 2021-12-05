## 安装

```shell
npm i -D @playwright/test
# install supported browsers
npx playwright install
```





## [First test](https://playwright.dev/docs/intro#first-test)

Create `tests/example.spec.js` (or `tests/example.spec.ts` for TypeScript) to define your test.

```js
const { test, expect } = require('@playwright/test');

test('basic test', async ({ page }) => {
  await page.goto('https://playwright.dev/');
  const title = page.locator('.navbar__inner .navbar__title');
  await expect(title).toHaveText('Playwright');
});
```



```bash
npx playwright test
npx playwright test --headed
```

## [Configuration file](https://playwright.dev/docs/intro#configuration-file)

*playwright.config.ts* 或者  *playwright.config.js* 

它允许您在根据需要配置的多个浏览器中运行测试。

```js
// playwright.config.js
// @ts-check
const { devices } = require('@playwright/test');

/** @type {import('@playwright/test').PlaywrightTestConfig} */
const config = {
  forbidOnly: !!process.env.CI,
  retries: process.env.CI ? 2 : 0,
  use: {
    trace: 'on-first-retry',
  },
  projects: [
    {
      name: 'chromium',
      use: { ...devices['Desktop Chrome'] },
    },
    {
      name: 'firefox',
      use: { ...devices['Desktop Firefox'] },
    },
    {
      name: 'webkit',
      use: { ...devices['Desktop Safari'] },
    },
  ],
};

module.exports = config;
```

Look for more options in the [configuration section](https://playwright.dev/docs/test-configuration).

Use `--project` command line option to run a single project.

```js
npx playwright test --project=firefox

Running 1 test using 1 worker

  ✓ [firefox] › example.spec.ts:3:1 › basic test (2s)
```

## Writing assertions[](https://playwright.dev/docs/inspector#writing-assertions)

Playwright Test uses [expect](https://jestjs.io/docs/expect) library for test assertions. It extends it with the Playwright-specific matchers to achieve greater testing ergonomics.

```js
// example.spec.js
const { test, expect } = require('@playwright/test');

test('my test', async ({ page }) => {
  await page.goto('https://playwright.dev/');

  // Expect a title "to contain" a substring.
  await expect(page).toHaveTitle(/Playwright/);

  // Expect an attribute "to be strictly equal" to the value.
  await expect(page.locator('text=Get Started').first()).toHaveAttribute('href', '/docs/intro');

  // Expect an element "to be visible".
  await expect(page.locator('text=Learn more').first()).toBeVisible();

  await page.click('text=Get Started');
  // Expect some text to be visible on the page.
  await expect(page.locator('text=Introduction').first()).toBeVisible();
});
```

## Using test fixtures[](https://playwright.dev/docs/inspector#using-test-fixtures)

You noticed an argument `{ page }` that the test above has access to:

 

```js
test('basic test', async ({ page }) => {
  ...
```

* 每个测试都会有 固定内置对象
* Playwright  会为每个 test申明 注入 相应固定对象

| Fixture     | Type                                                         | Description                                                  |
| ----------- | ------------------------------------------------------------ | ------------------------------------------------------------ |
| page        | [Page](https://playwright.dev/docs/api/class-page)           | Isolated page for this test run.                             |
| context     | [BrowserContext](https://playwright.dev/docs/api/class-browsercontext) | Isolated context for this test run. The `page` fixture belongs to this context as well. Learn how to [configure context](https://playwright.dev/docs/test-configuration). |
| browser     | [Browser](https://playwright.dev/docs/api/class-browser)     | Browsers are shared across tests to optimize resources. Learn how to [configure browser](https://playwright.dev/docs/test-configuration). |
| browserName | [string](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Data_structures#String_type) | The name of the browser currently running the test. Either `chromium`, `firefox` or `webkit`. |



## [Using test hooks](https://playwright.dev/docs/inspector#using-test-hooks)

**使用Hook**

```
// example.spec.js
const { test, expect } = require('@playwright/test');

test.describe('feature foo', () => {
  test.beforeEach(async ({ page }) => {
    // Go to the starting url before each test.
    await page.goto('https://playwright.dev/');
  });

  test('my test', async ({ page }) => {
    // Assertions use the expect API.
    await expect(page).toHaveURL('https://playwright.dev/');
  });
});
```

## Command line[](https://playwright.dev/docs/inspector#command-line)

- Run all the tests

  ```bash
  npx playwright test
  ```

  

- Run a single test file

  ```bash
  npx playwright test tests/todo-page.spec.ts
  ```

  

- Run a set of test files

  ```bash
  npx playwright test tests/todo-page/ tests/landing-page/
  ```

  

- Run files that have `my-spec` or `my-spec-2` in the file name

  ```bash
  npx playwright test my-spec my-spec-2
  ```

  

- Run the test with the title

  ```bash
  npx playwright test -g "add a todo item"
  ```

  

- Run tests in headed browsers

  ```bash
  npx playwright test --headed
  ```

  

- Run tests in a particular configuration (project)

  ```bash
  npx playwright test --project=firefox
  ```

  

- Disable [parallelization](https://playwright.dev/docs/test-parallel)

  ```bash
  npx playwright test --workers=1
  ```

  

- Choose a [reporter](https://playwright.dev/docs/test-reporters)

  ```bash
  npx playwright test --reporter=dot
  ```

  

- Run in debug mode with [Playwright Inspector](https://playwright.dev/docs/inspector)

  ```bash
  npx playwright test --debug
  ```

  

- Ask for help

  ```bash
  npx playwright test --help
  ```

## [Configure NPM scripts](https://playwright.dev/docs/inspector#configure-npm-scripts)

```js
{
  "scripts": {
    "test": "playwright test"
  }
}

{
  "scripts": {
    "test": "playwright test --config=tests/example.config.js"
  }
}
```


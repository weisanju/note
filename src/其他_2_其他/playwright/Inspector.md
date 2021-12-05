## 介绍

Playwright Inspector 是一个 GUI 工具 用来帮助 编写 和 调试 Playwright scripts

```
PWDEBUG=1 npm run test
```

Additional useful defaults are configured when `PWDEBUG=1` is set:

- Browsers launch in the headed mode
- Default timeout is set to 0 (= no timeout)

Call [page.pause()](https://playwright.dev/docs/api/class-page#page-pause) method from your script when running in headed browser.

```
// Pause on the following line.
await page.pause();
```



**使用 codegen 或者 open**

```bash
npx playwright codegen wikipedia.org
```

When `PWDEBUG=1` is set,

* Playwright Inspector window will be opened 
* the script will be paused on the first Playwright statement:



## [Debugging Selectors](https://playwright.dev/docs/inspector#debugging-selectors)

 



## Recording Scripting

```
npx playwright codegen wikipedia.org
```


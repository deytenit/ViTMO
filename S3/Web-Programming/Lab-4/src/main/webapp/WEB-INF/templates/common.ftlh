<#macro page>
    <!DOCTYPE html>
    <html lang="ru">
    <head>
        <meta charset="UTF-8">
        <title>Codeforces</title>
        <link rel="stylesheet" href="/css/normalize.css">
        <link rel="stylesheet" href="/css/style.css">
        <link rel="icon" href="/favicon.ico">
    </head>
    <body>
    <@header/>
    <div class="middle">
        <@sidebar data=posts trunc=150 />
        <main>
            <#nested/>
        </main>
    </div>
    <@footer/>
    </body>
    </html>
</#macro>

<#macro header>
    <header>
        <a href="#" class="logo"><img alt="Codeforces" title="Codeforces"
                                      src="/img/codeforces-logo-with-telegram.png"></a>
        <div class="languages">
            <a href="#"><img alt="In English" title="In English"
                             src="/img/gb.png"></a>
            <a href="#"><img alt="In Russian" title="In Russian"
                             src="/img/ru.png"></a>
        </div>
        <div class="enter-or-register-box">
            <#if user??>
                <@userlink user/>
                |
                <a href="/logout">Logout</a>
            <#else>
                <a href="/enter">Enter</a>
                |
                <a href="/register">Register</a>
            </#if>
        </div>
        <nav>
            <#assign items=[
            {"href": "/index", "name": "Home"},
            {"href": "/misc/help", "name": "Help"},
            {"href": "/contests", "name": "Contests"},
            {"href": "/users", "name": "Users"}
            ]/>
            <ul>
                <#list items as item>
                    <li>
                        <a ${if(item.href == href, "class=underlined")} href="${item.href}">${item.name}</a>
                    </li>
                </#list>
            </ul>
        </nav>
    </header>
</#macro>

<#macro sidebar data trunc=0>
    <aside>
        <#list data as p>
            <section>
                <div class="header">Post #${p.id}</div>
                <div class="body">${if(trunc != 0, p.text?truncate(trunc, "..."), p.text)}</div>
                <div class="footer">
                    <a href="/post?post_id=${p.id}">View all</a>
                </div>
            </section>
        </#list>
    </aside>
</#macro>

<#macro footer>
    <footer>
        2010-2021 &copy; by Mike Mirzayanov
    </footer>
</#macro>

<#macro userlink user colored=false>
    <#-- @ftlvariable name="user" type="ru.itmo.web.hw4.model.User" -->
    <#if colored>
        <a href="/user?handle=${user.handle}" class="user-link" style="color: ${user.color.css};">${user.name}</a>
    <#else>
        <a href="/user?handle=${user.handle}">${user.name}</a>
    </#if>
</#macro>

<#macro post data trunc=0>
    <#assign u = users[findBy(users, "id", data.user_id)] />

    <article>
        <div class="title">
            <#if trunc == 0>
                ${data.title}
            <#else>
                <a href="/post?post_id=${data.id}">${data.title}</a>
            </#if>
        </div>
        <div class="information">By <@userlink u true/></div>
        <div class="body">
            <#list if(trunc != 0, data.text?truncate(trunc, "..."), data.text)?split("\n") as p>
                <p>${p}</p>
            </#list>
        </div>
        <div class="footer">
            <div class="left">
                <img src="img/voteup.png" title="Vote Up" alt="Vote Up"/>
                <span>0</span>
                <img src="img/votedown.png" title="Vote Down" alt="Vote Down"/>
            </div>
            <div class="right">
                <img src="img/date_16x16.png" title="Publish Time" alt="Publish Time"/>
                0
                <img src="img/comments_16x16.png" title="Comments" alt="Comments"/>
                <a href="#">0</a>
            </div>
        </div>
    </article>
</#macro>

<#function findBy items key value>
    <#list items as item>
        <#if item[key] == value>
            <#return item?index/>
        </#if>
    </#list>
    <#return -1/>
</#function>

<#function if condition then else="">
    <#if condition>
        <#return then>
    <#else>
        <#return else>
    </#if>
</#function>
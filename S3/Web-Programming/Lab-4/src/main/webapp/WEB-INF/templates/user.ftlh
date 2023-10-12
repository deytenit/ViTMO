<#-- @ftlvariable name="handle" type="java.lang.String" -->
<#-- @ftlvariable name="user_id" type="java.lang.Long" -->
<#import "./common.ftlh" as c>

<@c.page>
    <#assign idx = c.findBy(users, c.if(handle??, "handle", "id"), handle!user_id)/>

    <div class="user-selector-box">
    <div class="profile">
    <#if idx != -1 >
        <div class="data">
            <span class="name">
                <a href="#">${users[idx].name}</a>
            </span>

            <ul>
                <li>id: ${users[idx].id}</li>
                <li>handle: ${users[idx].handle}</li>
                <li>name: ${users[idx].name}</li>
                <li>posts: <a href="/posts?user_id=${users[idx].id}">${posts?filter(x -> x.user_id == users[idx].id)?size}</a></li>
            </ul>
        </div>

        <div class="avatar">
            <img src="https://userpic.codeforces.org/no-title.jpg" alt="no-image"/>
        </div>
    <#else>
        No such user
    </#if>
    </div>
    <div class="footer">
        <ul>
            <li>
            <#if users[idx - 1]?has_content>
                <a href="?handle=${users[idx - 1].handle}">←</a>
            <#else>
                ←
            </#if>
            <li>
                <#if users[idx + 1]?has_content>
                    <a href="?handle=${users[idx + 1].handle}">→</a>
                <#else>
                    →
                </#if>
            </li>
        </ul>
    </div>
    </div>

</@c.page>

<#import "commons.ftlh" as c>

<@c.page>
    <div class="profile">
        <#if user?? >
            <div class="data">
            <span class="name">
                <a href="#">${user.login}</a>
            </span>

                <ul>
                    <li>id: ${user.id}</li>
                    <li>login: ${user.login}</li>
                    <li>creation time: ${user.creationTime}</li>
                </ul>
            </div>

            <div class="avatar">
                <img src="https://userpic.codeforces.org/no-title.jpg" alt="no-image"/>
            </div>
        <#else>
            No such user
        </#if>
    </div>
</@c.page>

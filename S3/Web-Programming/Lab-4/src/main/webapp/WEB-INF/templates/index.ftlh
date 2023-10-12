<#import "./common.ftlh" as c>

<@c.page>
    <#list posts?reverse as p>
        <@c.post p 350 />
    </#list>
</@c.page>

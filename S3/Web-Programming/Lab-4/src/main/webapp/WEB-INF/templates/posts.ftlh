<#-- @ftlvariable name="user_id" type="java.lang.Long" -->
<#import "./common.ftlh" as c>

<@c.page>
    <#assign filtered = posts?filter(x -> x.user_id == user_id)?reverse />

    <#if filtered?has_content >
        <#list filtered as p>
            <@c.post p 350 />
        </#list>
    <#else>
        User has no posts
    </#if>
</@c.page>

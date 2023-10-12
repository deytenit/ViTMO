<#-- @ftlvariable name="post_id" type="java.lang.Long" -->
<#import "./common.ftlh" as c>

<@c.page>
    <#assign idx = c.findBy(posts, "id", post_id)/>

    <#if idx != -1>
        <@c.post posts[idx] />
    <#else>
        No such post
    </#if>

</@c.page>

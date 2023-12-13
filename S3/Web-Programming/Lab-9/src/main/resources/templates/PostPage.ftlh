<#-- @ftlvariable name="post" type="ru.itmo.wp.domain.Post" -->
<#-- @ftlvariable name="comment" type="ru.itmo.wp.domain.Comment" -->
<#import "commons.ftlh" as c>

<@c.page>
    <#if post??>
    <@c.post post=post full=true/>
    <div class="comments-box">
        <#if user??>
            <table>
                <tbody>
                <tr>
                    <td class="user">
                        <img src="https://userpic.codeforces.org/no-title.jpg">
                        <span>${user.login}</span>
                    </td>
                    <td class="form-box">
                        <form method="post" action="">
                            <input type="hidden" name="action" value="comment" />
                            <textarea autofocus name="text" id="text">${comment.text!}</textarea>
                            <@c.error "comment.text" />
                            <input type="submit" value="Comment" />
                        </form>
                    </td>
                </tr>
                </tbody>
            </table>
        </#if>
        <#list post.comments as comment>
            <div class="comment">
                <table>
                    <tbody>
                    <tr>
                        <td class="user">
                            <img src="https://userpic.codeforces.org/no-title.jpg">
                            <span>${comment.user.login}</span>
                        </td>
                        <td class="content">
                            <div class="information">${comment.creationTime}</div>
                            <br/>
                            <div>
                                <@c.paragraphs comment.text />
                            </div>
                        </td>
                    </tr>
                    </tbody>
                </table>
            </div>
        </#list>
    </div>
    <#else>
        No Such Post.
    </#if>
</@c.page>

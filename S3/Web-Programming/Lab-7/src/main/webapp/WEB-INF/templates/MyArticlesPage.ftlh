<#-- @ftlvariable name="articles" type="ru.itmo.wp.model.domain.Article[]" -->
<#import "commons.ftlh" as c>

<@c.page>
    <div class="articles datatable">
        <div class="caption">User</div>
        <table>
            <thead>
            <tr>
                <th>Id</th>
                <th>Title</th>
                <th>Hide/Show</th>
            </tr>
            </thead>
            <tbody>
            <#list articles as article>
                <tr>
                    <td class="article-id">${article.id}</td>
                    <td class="article-title">${article.title}</td>
                    <td class="article-button"><button>${c.if(article.hidden, "show", "hide")}</button></td>
                </tr>
            </#list>
            </tbody>
        </table>
    </div>

    <script>
        $(function () {
            $(".articles table tbody").children().each(function () {
                const articleId = $(this).find(".article-id").text();

                const $button = $(this).find(".article-button button");
                $button.click(function () {
                    ajax(
                        { action: "updateArticleHidden", id: articleId, hidden: $button.text() === "hide" },
                        (response) => {
                            $button.text(response.hidden ? "show" : "hide");
                            notify("Article has been updated!");
                        }
                    );
                });
            });
        });
    </script>
</@c.page>

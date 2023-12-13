<#import "commons.ftlh" as c>

<@c.page>
    <template>
        <article>
            <div class="title"></div>
            <div class="information"></div>
            <div class="body">
            </div>
            <ul class="attachment">
            </ul>
            <div class="footer">
                <div class="left">
                    <img src="img/voteup.png" title="Vote Up" alt="Vote Up"/>
                    <span class="positive-score">0</span>
                    <img src="img/votedown.png" title="Vote Down" alt="Vote Down"/>
                </div>
                <div class="right">
                    <img src="img/date_16x16.png" title="Publish Time" alt="Publish Time"/>
                    <span class="creation-time"></span>
                    <img src="img/comments_16x16.png" title="Comments" alt="Comments"/>
                    <a href="#">0</a>
                </div>
            </div>
        </article>
    </template>
    <div class="noData"></div>
    <script>
        $(function () {
            const $noData = $("main .noData");
            $noData.text("Loading...");

            ajax({ action: "findAllArticlesWithLogins" }, (response) => {
                response.result.forEach((article) => {
                    $noData.hide();
                    const $article = $($("main template").prop("content")).clone();

                    $article.find(".title").text(article.title);
                    $article.find(".information").text(`By <#noparse>${article.userLogin}, ${article.creationTime}</#noparse>, translation`)
                    $article.find(".body").append(...article.text.split("\n").map((paragraph) => $("<p></p>").text(paragraph)));
                    $article.find(".footer .creation-time").text(article.creationTime);

                    $("main").append($article);
                });
            });
        });
    </script>
</@c.page>

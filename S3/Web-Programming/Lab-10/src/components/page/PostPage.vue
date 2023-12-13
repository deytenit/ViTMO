<template>
  <div>
    <ArticlePost :post="post" :full="true" />
    <Comments :post-id="post.id" :comments="filteredComments" :user="user" />
  </div>
</template>

<script>
import ArticlePostBox from "../article/ArticlePostBox";
import CommentsBox from "../article/CommentsBox";

export default {
  name: "PostPage",
  components: {
    ArticlePost: ArticlePostBox,
    Comments: CommentsBox
  },
  props: ["post", "comments", "user"],
  computed: {
    filteredComments: function () {
      return this.comments.filter((x) => x.postId === this.post.id).sort((a, b) => b.id - a.id);
    }
  }
};
</script>

<style scoped></style>

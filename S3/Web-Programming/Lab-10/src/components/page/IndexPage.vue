<template>
  <div>
    <ArticlePost v-for="post in posts" :key="post.id" :post="post" :full="false" />
  </div>
</template>

<script>
import ArticlePostBox from "../article/ArticlePostBox";

export default {
  name: "IndexPage",
  components: {
    ArticlePost: ArticlePostBox
  },
  props: ["posts"]
};
</script>

<style scoped></style>

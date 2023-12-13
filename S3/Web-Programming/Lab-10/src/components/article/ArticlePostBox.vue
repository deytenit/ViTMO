<template>
  <article>
    <div v-if="full" class="title">{{ post.title }}</div>
    <div v-else class="title">
      <a href="#" @click.prevent="changePage('Post', post.id)">{{ post.title }}</a>
    </div>
    <div class="information">By {{ post.user.name }}</div>
    <div class="body">
      <Paragraphs :text="post.text" :truncate="!full" :key-prefix="post.id" />
    </div>
    <div class="footer">
      <div class="left">
        <img src="../../assets/img/voteup.png" title="Vote Up" alt="Vote Up" />
        <span class="positive-score">0</span>
        <img src="../../assets/img/votedown.png" title="Vote Down" alt="Vote Down" />
      </div>
      <div class="right">
        <img src="../../assets/img/comments_16x16.png" title="Comments" alt="Comments" />
        <span v-if="full">{{ post.commentCount }}</span>
        <a v-else href="#" @click.prevent="changePage('Post', post.id)">{{ post.commentCount }}</a>
      </div>
    </div>
  </article>
</template>

<script>
import ParagraphsBox from "../misc/ParagraphsBox";

export default {
  name: "ArticlePostBox",
  components: { Paragraphs: ParagraphsBox },
  props: ["post", "full"],
  methods: {
    changePage: function (page, swap) {
      this.$root.$emit("onChangePage", page, swap);
    }
  }
};
</script>

<style scoped></style>

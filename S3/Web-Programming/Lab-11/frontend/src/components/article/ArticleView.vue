<template>
  <article>
    <div v-if="full" class="title">{{ post.title }}</div>
    <div v-else class="title">
      <a href="#" @click.prevent="changePage('Post', swap)">{{ post.title }}</a>
    </div>
    <div class="information">By {{ post.user.name }}</div>
    <div class="body">
      <ParagraphsView :text="post.text" :truncate="!full" />
    </div>
    <div class="footer">
      <div class="left">
        <img src="../../assets/img/voteup.png" title="Vote Up" alt="Vote Up" />
        <span class="positive-score">0</span>
        <img src="../../assets/img/votedown.png" title="Vote Down" alt="Vote Down" />
      </div>
      <div class="right">
        <img src="../../assets/img/comments_16x16.png" title="Comments" alt="Comments" />
        <span v-if="full">{{ commentCount }}</span>
        <a v-else href="#" @click.prevent="changePage('Post', swap)">{{ commentCount }}</a>
      </div>
    </div>
  </article>
</template>

<script>
import axios from "axios";

import ParagraphsView from "../misc/ParagraphsView";

export default {
  name: "ArticleView",
  components: { ParagraphsView },
  props: ["post", "commentCount", "full"],
  methods: {
    changePage: function (page, swap) {
      this.$root.$emit("onChangePage", page, swap);
    },
    swap(that) {
      axios.get(`/api/1/post/${this.post.id}`).then((response) => {
        that.swap = response.data;
      });
    }
  }
};
</script>

<style scoped>
article {
  overflow-wrap: break-word;
}
</style>

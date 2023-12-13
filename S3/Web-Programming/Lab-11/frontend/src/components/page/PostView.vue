<template>
  <div v-if="!post?.id">Loading...</div>
  <div v-else>
    <ArticleView :post="post" :comment-count="comments.length" :full="full" />
    <CommentsView
      v-if="full"
      :post-id="post.id"
      :comments="comments"
      :user="user"
      :update-fn="update"
    />
  </div>
</template>

<script>
import axios from "axios";
import ArticleView from "../article/ArticleView";
import CommentsView from "../article/CommentsView";

export default {
  name: "PostView",
  components: { ArticleView, CommentsView },
  props: ["post", "full", "user"],
  data() {
    return {
      comments: []
    };
  },
  watch: {
    post() {
      this.update();
    }
  },
  beforeMount() {
    this.update();
  },
  methods: {
    update() {
      if (this.post?.id) {
        axios.get(`/api/1/post/${this.post.id}/comments`).then((response) => {
          this.comments = response.data.sort((a, b) => b.id - a.id);
        });
      }
    }
  }
};
</script>

<style scoped></style>

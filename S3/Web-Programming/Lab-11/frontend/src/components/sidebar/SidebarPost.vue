<template>
  <section>
    <div class="header">
      {{ post.title }}
    </div>
    <div class="body">
      <ParagraphsView :text="post.text" :truncate="true" />
    </div>
    <div class="footer">
      <a href="#" @click.prevent="changePage('Post', swap)">View all</a>
    </div>
  </section>
</template>

<script>
import axios from "axios";
import ParagraphsView from "../misc/ParagraphsView";

export default {
  name: "SidebarPost",
  components: { ParagraphsView },
  props: ["post"],
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
section {
  margin-bottom: 1rem;
  overflow-wrap: break-word;
}
</style>

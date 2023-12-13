<template>
  <section>
    <div class="header">
      {{ post.title }}
    </div>
    <div class="body">
      {{ post.text }}
    </div>
    <div class="footer">
      <a href="#" @click.prevent="changePage('Post', post.id)">View all</a>
    </div>
  </section>
</template>

<script>
export default {
  name: "SidebarPostBox",
  props: ["post"],
  methods: {
    changePage: function (page, swap) {
      this.$root.$emit("onChangePage", page, swap);
    }
  }
};
</script>

<style scoped>
section {
  margin-bottom: 1rem;
}
</style>

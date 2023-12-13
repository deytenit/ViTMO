<template>
  <div>
    <p v-for="(par, idx) in paragraphs" :key="idx">{{ par }}</p>
  </div>
</template>

<script>
export default {
  name: "ParagraphsView",
  props: ["text", "truncate"],
  computed: {
    paragraphs: function () {
      return (this.truncate ? this.text.substring(0, 300) : this.text).split("\n");
    }
  }
};
</script>

<style scoped></style>

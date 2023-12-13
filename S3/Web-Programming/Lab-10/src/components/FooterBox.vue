<template>
  <footer>
    <a href="/">Codeforces</a> &copy; 2022 by Mike Mirzayanov<br />{{ userCount }} users
    registered!<br />{{ postCount }} posts was written!
  </footer>
</template>

<script>
export default {
  name: "FooterBox",
  props: ["userCount", "postCount"]
};
</script>

<style scoped></style>

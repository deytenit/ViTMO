<template>
  <div class="users datatable">
    <div class="caption">User</div>
    <table>
      <thead>
        <tr>
          <th v-for="key in keys" :key="`users-titles-${key}`">{{ titles[key] }}</th>
        </tr>
      </thead>
      <tbody>
        <tr v-if="!data || Object.keys(data).length === 0" class="noData">
          <td colspan="3">No data</td>
        </tr>
        <tr v-for="(row, index) in data" v-else :key="`users-row-${index}`">
          <td v-for="key in keys" :key="`users-data-${key}`" :class="key">{{ row[key] }}</td>
        </tr>
      </tbody>
    </table>
  </div>
</template>

<script>
export default {
  name: "DatatableBox",
  props: ["keys", "titles", "data"]
};
</script>

<style scoped></style>

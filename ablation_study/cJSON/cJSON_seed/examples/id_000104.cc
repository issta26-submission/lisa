#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 104
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    char *name1 = (char *)cJSON_malloc(6);
    name1[0] = 'A'; name1[1] = 'l'; name1[2] = 'i'; name1[3] = 'c'; name1[4] = 'e'; name1[5] = '\0';
    char *name2 = (char *)cJSON_malloc(4);
    name2[0] = 'B'; name2[1] = 'o'; name2[2] = 'b'; name2[3] = '\0';
    char *name3 = (char *)cJSON_malloc(6);
    name3[0] = 'C'; name3[1] = 'a'; name3[2] = 'r'; name3[3] = 'o'; name3[4] = 'l'; name3[5] = '\0';
    const char *const names_arr[3] = { name1, name2, name3 };
    cJSON *names = cJSON_CreateStringArray(names_arr, 3);
    cJSON_AddItemToObject(root, "names", names);

    // step 2: Configure
    char *raw = (char *)cJSON_malloc(16);
    raw[0] = ' '; raw[1] = '{'; raw[2] = ' '; raw[3] = '"'; raw[4] = 'k'; raw[5] = '"';
    raw[6] = ' '; raw[7] = ':'; raw[8] = ' '; raw[9] = '"'; raw[10] = 'v'; raw[11] = '"';
    raw[12] = ' '; raw[13] = '}'; raw[14] = ' '; raw[15] = '\0';
    cJSON_Minify(raw);
    cJSON *minified_str = cJSON_CreateString(raw);
    cJSON_AddItemToObject(root, "minified", minified_str);
    char *greet_buf = (char *)cJSON_malloc(12);
    greet_buf[0] = 'h'; greet_buf[1] = 'e'; greet_buf[2] = 'l'; greet_buf[3] = 'l'; greet_buf[4] = 'o';
    greet_buf[5] = '_'; greet_buf[6] = 'w'; greet_buf[7] = 'o'; greet_buf[8] = 'r'; greet_buf[9] = 'l';
    greet_buf[10] = 'd'; greet_buf[11] = '\0';
    cJSON *greeting = cJSON_CreateString(greet_buf);
    cJSON_AddItemToObject(root, "greeting", greeting);

    // step 3: Operate & Validate
    const char *got_greet = cJSON_GetStringValue(greeting);
    cJSON *greeting_copy = cJSON_CreateString(got_greet);
    cJSON_AddItemToObject(root, "greeting_copy", greeting_copy);
    cJSON_bool has_greet = cJSON_HasObjectItem(root, "greeting");
    cJSON_bool has_names = cJSON_HasObjectItem(root, "names");
    cJSON *flag_greet = cJSON_CreateNumber((double)has_greet);
    cJSON *flag_names = cJSON_CreateNumber((double)has_names);
    cJSON_AddItemToObject(root, "has_greeting", flag_greet);
    cJSON_AddItemToObject(root, "has_names", flag_names);

    // step 4: Cleanup
    cJSON_free(name1);
    cJSON_free(name2);
    cJSON_free(name3);
    cJSON_free(raw);
    cJSON_free(greet_buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
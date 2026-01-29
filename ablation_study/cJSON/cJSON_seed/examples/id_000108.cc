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
//<ID> 108
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const char *strings[] = { "alpha", "beta", "gamma" };
    cJSON *str_array = cJSON_CreateStringArray(strings, 3);
    cJSON_AddItemToObject(root, "strings", str_array);

    // step 2: Configure
    char *min_buf = (char *)cJSON_malloc(22);
    min_buf[0] = ' ';
    min_buf[1] = ' ';
    min_buf[2] = '{';
    min_buf[3] = '"';
    min_buf[4] = 'i';
    min_buf[5] = 'n';
    min_buf[6] = 'n';
    min_buf[7] = 'e';
    min_buf[8] = 'r';
    min_buf[9] = '"';
    min_buf[10] = ':';
    min_buf[11] = '"';
    min_buf[12] = 'v';
    min_buf[13] = 'a';
    min_buf[14] = 'l';
    min_buf[15] = 'u';
    min_buf[16] = 'e';
    min_buf[17] = '"';
    min_buf[18] = '}';
    min_buf[19] = ' ';
    min_buf[20] = ' ';
    min_buf[21] = '\0';
    cJSON_Minify(min_buf);
    cJSON *minified_item = cJSON_CreateString(min_buf);
    cJSON_AddItemToObject(root, "minified", minified_item);
    cJSON_free(min_buf);

    // step 3: Operate & Validate
    cJSON_bool has_strings = cJSON_HasObjectItem(root, "strings");
    cJSON_AddItemToObject(root, "has_strings", cJSON_CreateBool(has_strings));
    cJSON *first_item = cJSON_GetArrayItem(str_array, 0);
    const char *first_text = cJSON_GetStringValue(first_item);
    cJSON_AddItemToObject(root, "first", cJSON_CreateString(first_text));
    cJSON_bool has_minified = cJSON_HasObjectItem(root, "minified");
    cJSON_AddItemToObject(root, "has_minified", cJSON_CreateNumber((double)has_minified));

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
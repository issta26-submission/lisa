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
//<ID> 102
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *names_list[3] = { "Alice", "Bob", "Carol" };
    cJSON *root = cJSON_CreateObject();
    cJSON *names_arr = cJSON_CreateStringArray(names_list, 3);
    cJSON_AddItemToObject(root, "names", names_arr);

    // step 2: Configure
    char *buf = (char *)cJSON_malloc(34);
    buf[0] = ' '; buf[1] = ' '; buf[2] = '{'; buf[3] = ' '; buf[4] = '"'; buf[5] = 'm'; buf[6] = 'i'; buf[7] = 'n'; buf[8] = '"';
    buf[9] = ' '; buf[10] = ':'; buf[11] = ' '; buf[12] = '['; buf[13] = ' '; buf[14] = '"'; buf[15] = 'o'; buf[16] = 'n'; buf[17] = 'e';
    buf[18] = '"'; buf[19] = ' '; buf[20] = ','; buf[21] = ' '; buf[22] = '"'; buf[23] = 't'; buf[24] = 'w'; buf[25] = 'o'; buf[26] = '"';
    buf[27] = ' '; buf[28] = ']'; buf[29] = ' '; buf[30] = '}'; buf[31] = ' '; buf[32] = ' '; buf[33] = '\0';
    cJSON_Minify(buf);
    cJSON *minified_str = cJSON_CreateString(buf);
    cJSON_AddItemToObject(root, "minified", minified_str);

    // step 3: Operate & Validate
    cJSON_bool has_names = cJSON_HasObjectItem(root, "names");
    cJSON *has_num = cJSON_CreateNumber((double)has_names);
    cJSON_AddItemToObject(root, "has_names", has_num);
    cJSON *first = cJSON_GetArrayItem(names_arr, 0);
    const char *first_text = cJSON_GetStringValue(first);
    cJSON *first_copy = cJSON_CreateString(first_text);
    cJSON_AddItemToObject(root, "first_name_copy", first_copy);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
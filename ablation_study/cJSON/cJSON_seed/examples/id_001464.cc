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
//<ID> 1464
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"num\":123.5,\"text\":\"hello\"}";
    cJSON *root = cJSON_Parse(json);
    const char *ver = cJSON_Version();
    cJSON *added_str = cJSON_CreateString("appended");

    // step 2: Configure
    cJSON_AddItemToObject(root, ver, added_str);
    cJSON *num_item = cJSON_GetObjectItem(root, "num");
    cJSON *text_item = cJSON_GetObjectItem(root, "text");

    // step 3: Operate & Validate
    cJSON_bool num_is_num = cJSON_IsNumber(num_item);
    double num_value = cJSON_GetNumberValue(num_item);
    const char *got_text = cJSON_GetStringValue(text_item);
    double computed = num_value * (double)num_is_num;
    (void)got_text;
    (void)computed;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
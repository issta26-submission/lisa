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
//<ID> 1278
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *created_num = NULL;
    cJSON *added_num = NULL;
    cJSON *null_item = NULL;
    cJSON *str_item = NULL;
    cJSON *got_num = NULL;
    const char *got_str = NULL;
    double got_number = 0.0;
    char *printed = NULL;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    created_num = cJSON_CreateNumber(3.14159);
    cJSON_AddItemToObject(root, "created_pi", created_num);
    added_num = cJSON_AddNumberToObject(root, "added_answer", 42.0);
    null_item = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "missing", null_item);
    cJSON_AddStringToObject(root, "greeting", "hello world");

    // step 3: Operate / Validate
    str_item = cJSON_GetObjectItem(root, "greeting");
    got_str = cJSON_GetStringValue(str_item);
    got_num = cJSON_GetObjectItem(root, "added_answer");
    got_number = cJSON_GetNumberValue(got_num);
    printed = cJSON_PrintUnformatted(root);
    (void)got_str;
    (void)got_number;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
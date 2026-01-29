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
//<ID> 2154
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    char buf[32];
    memset(buf, 0, sizeof(buf));
    cJSON *root = NULL;
    cJSON *child = NULL;
    cJSON *raw = NULL;
    cJSON *num_item = NULL;
    cJSON *str_item = NULL;
    cJSON *child_num = NULL;
    cJSON *child_str = NULL;
    char *printed = NULL;
    cJSON_bool added_child_cs = 0;
    cJSON_bool added_raw_cs = 0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();
    raw = cJSON_CreateRaw("{\"embedded\":true}");

    // step 3: Configure (populate child and root)
    child_num = cJSON_AddNumberToObject(child, "child_count", 7.0);
    child_str = cJSON_AddStringToObject(child, "child_name", "inner");
    num_item = cJSON_AddNumberToObject(root, "count", 3.0);
    str_item = cJSON_AddStringToObject(root, "name", "test");

    // step 4: Operate (attach child and raw using case-sensitive add, then print)
    added_child_cs = cJSON_AddItemToObjectCS(root, "child_cs", child);
    added_raw_cs = cJSON_AddItemToObjectCS(root, "rawCS", raw);
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(child != NULL);
    validation ^= (int)(raw != NULL);
    validation ^= (int)(num_item != NULL);
    validation ^= (int)(str_item != NULL);
    validation ^= (int)(child_num != NULL);
    validation ^= (int)(child_str != NULL);
    validation ^= (int)(added_child_cs != 0);
    validation ^= (int)(added_raw_cs != 0);
    validation ^= (int)(printed != NULL);
    buf[0] = (char)(validation + '0');

    // step 6: Cleanup
    cJSON_Delete(root);
    cJSON_free(printed);

    // API sequence test completed successfully
    return 66;
}
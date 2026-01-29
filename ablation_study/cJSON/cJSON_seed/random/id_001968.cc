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
//<ID> 1968
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    char buf[64];
    memset(buf, 0, sizeof(buf));
    cJSON *root = NULL;
    cJSON *got_num = NULL;
    cJSON *str_item = NULL;
    char *out = NULL;
    void *tmp_mem = NULL;
    double num_val = 0.0;
    cJSON_bool added_str = 0;
    cJSON *added_flag = NULL; // fixed type: cJSON_AddBoolToObject returns cJSON*
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "pi", 3.14);
    str_item = cJSON_CreateString("example");
    added_str = cJSON_AddItemToObject(root, "name", str_item);
    added_flag = cJSON_AddBoolToObject(root, "enabled", 1);
    tmp_mem = cJSON_malloc(128);
    memset(tmp_mem, 0, 128);

    // step 3: Core operations
    out = cJSON_PrintUnformatted(root);
    got_num = cJSON_GetObjectItem(root, "pi");
    num_val = cJSON_GetNumberValue(got_num);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(str_item != NULL);
    validation ^= (int)(added_str != 0);
    validation ^= (int)(added_flag != NULL);
    validation ^= (int)(out != NULL);
    validation ^= (int)(num_val == 3.14);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(out);
    cJSON_free(tmp_mem);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
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
//<ID> 1969
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
    cJSON *str_item = NULL;
    cJSON *bool_item = NULL;
    cJSON *got_num = NULL;
    char *out = NULL;
    char *dup_buf = NULL;
    double num_val = 0.0;
    cJSON_bool added_str = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "value", 42.0);
    str_item = cJSON_CreateString("sample");
    added_str = cJSON_AddItemToObject(root, "text", str_item);
    bool_item = cJSON_AddBoolToObject(root, "flag", 1);

    // step 3: Core operations
    out = cJSON_PrintUnformatted(root);
    got_num = cJSON_GetObjectItem(root, "value");
    num_val = cJSON_GetNumberValue(got_num);
    dup_buf = (char*)cJSON_malloc(8);
    dup_buf[0] = out[0];
    dup_buf[1] = '\0';

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(str_item != NULL);
    validation ^= (int)(bool_item != NULL);
    validation ^= (int)(added_str != 0);
    validation ^= (int)(out != NULL);
    validation ^= (int)(num_val == 42.0);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(dup_buf);
    cJSON_free(out);
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}
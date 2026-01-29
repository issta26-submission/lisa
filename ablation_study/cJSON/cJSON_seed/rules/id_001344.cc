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
//<ID> 1344
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *num = NULL;
    cJSON *str = NULL;
    cJSON *t = NULL;
    char *out1 = NULL;
    char *out2 = NULL;
    cJSON_bool added_num = 0;
    cJSON_bool added_str = 0;
    cJSON_bool added_true = 0;
    double num_val = 42.0;
    const char *str_val = "hello cJSON";

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    num = cJSON_CreateNumber(num_val);
    str = cJSON_CreateString(str_val);
    t = cJSON_CreateTrue();
    added_num = cJSON_AddItemToObject(root, "answer", num);
    added_str = cJSON_AddItemToObject(root, "greeting", str);
    added_true = cJSON_AddItemToObject(root, "flag", t);

    // step 3: Operate / Validate
    out1 = cJSON_PrintBuffered(root, 128, 1);
    cJSON_DeleteItemFromObjectCaseSensitive(root, "flag");
    out2 = cJSON_PrintBuffered(root, 128, 1);
    (void)added_num;
    (void)added_str;
    (void)added_true;
    (void)out1;
    (void)out2;
    (void)num_val;
    (void)str_val;

    // step 4: Cleanup
    cJSON_free(out1);
    cJSON_free(out2);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}
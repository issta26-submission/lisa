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
//<ID> 9
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = cJSON_CreateObject();
    cJSON *null_item = NULL;
    cJSON *true_item = NULL;
    cJSON *raw_item = NULL;
    cJSON *dup = NULL;
    cJSON *retr_true = NULL;
    cJSON *retr_raw = NULL;
    char *printed = NULL;
    int validation = 0;

    // step 2: Setup
    null_item = cJSON_AddNullToObject(root, "nullable");
    true_item = cJSON_AddTrueToObject(root, "flag");
    raw_item = cJSON_CreateRaw("{\"embedded\":true}");
    cJSON_AddItemToObject(root, "rawdata", raw_item);
    cJSON_AddStringToObject(root, "name", "cJSON_test");
    cJSON_AddNumberToObject(root, "initial", 1.0);

    // step 3: Operate/Validate
    retr_true = cJSON_GetObjectItem(root, "flag");
    retr_raw = cJSON_GetObjectItem(root, "rawdata");
    validation = (int)cJSON_IsTrue(retr_true) + 2 * (int)cJSON_IsRaw(retr_raw);
    dup = cJSON_Duplicate(root, 1);
    cJSON_AddNumberToObject(dup, "validation", (double)validation);

    // step 4: Cleanup
    printed = cJSON_PrintUnformatted(dup);
    cJSON_free(printed);
    cJSON_Delete(dup);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
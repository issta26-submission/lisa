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
//<ID> 1239
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *true_item = (cJSON *)0;
    cJSON *raw_item = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool raw_flag = 0;
    cJSON_bool true_is_raw = 0;
    int validation_score = 0;

    // step 2: Initialize - create an object root and an array container and items
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    true_item = cJSON_CreateTrue();
    raw_item = cJSON_CreateRaw("RAW_CONTENT");

    // step 3: Configure - attach items to array and attach array to root
    cJSON_AddItemToArray(arr, true_item);
    cJSON_AddItemToArray(arr, raw_item);
    cJSON_AddItemToObject(root, "payload", arr);

    // step 4: Operate - inspect raw-ness and produce a buffered print of the root
    raw_flag = cJSON_IsRaw(raw_item);
    true_is_raw = cJSON_IsRaw(true_item);
    printed = cJSON_PrintBuffered(root, 256, 1);

    // step 5: Validate and Cleanup - compute a small validation score, free print buffer and delete root
    validation_score = (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)(true_item != (cJSON *)0) + (int)(raw_item != (cJSON *)0) + (int)(printed != (char *)0) + (int)raw_flag + (int)(!true_is_raw);
    (void)validation_score;
    if (printed) { cJSON_free(printed); }
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
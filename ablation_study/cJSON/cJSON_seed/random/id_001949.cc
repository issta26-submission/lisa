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
//<ID> 1949
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
    cJSON *arr = NULL;
    cJSON *n1 = NULL;
    cJSON *n2 = NULL;
    cJSON *got_count = NULL;
    cJSON *got_item0 = NULL;
    char *out = NULL;
    double count_val = 0.0;
    double item0_val = 0.0;
    cJSON_bool added_arr = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "count", 2.0);
    arr = cJSON_CreateArray();
    n1 = cJSON_CreateNumber(10.0);
    n2 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    added_arr = cJSON_AddItemToObject(root, "items", arr);

    // step 3: Core operations
    out = cJSON_PrintUnformatted(root);
    got_count = cJSON_GetObjectItem(root, "count");
    count_val = cJSON_GetNumberValue(got_count);
    got_item0 = cJSON_GetArrayItem(arr, 0);
    item0_val = cJSON_GetNumberValue(got_item0);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(n1 != NULL);
    validation ^= (int)(n2 != NULL);
    validation ^= (int)(added_arr != 0);
    validation ^= (int)(out != NULL);
    validation ^= (int)(count_val == 2.0);
    validation ^= (int)(item0_val == 10.0);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}
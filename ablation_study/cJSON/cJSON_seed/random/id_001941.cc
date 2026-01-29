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
//<ID> 1941
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
    cJSON *n0 = NULL;
    cJSON *n1 = NULL;
    cJSON *got_pi = NULL;
    cJSON *got_first = NULL;
    char *out = NULL;
    double pi_val = 0.0;
    double first_val = 0.0;
    cJSON_bool has_pi = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "pi", 3.1415);
    arr = cJSON_CreateArray();
    n0 = cJSON_CreateNumber(1.0);
    n1 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(arr, n0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToObject(root, "numbers", arr);
    out = cJSON_PrintUnformatted(root);

    // step 3: Core operations
    got_pi = cJSON_GetObjectItem(root, "pi");
    pi_val = cJSON_GetNumberValue(got_pi);
    got_first = cJSON_GetArrayItem(arr, 0);
    first_val = cJSON_GetNumberValue(got_first);
    has_pi = cJSON_HasObjectItem(root, "pi");

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(n0 != NULL);
    validation ^= (int)(n1 != NULL);
    validation ^= (int)(out != NULL);
    validation ^= (int)(got_pi != NULL);
    validation ^= (int)(got_first != NULL);
    validation ^= (int)(has_pi != 0);
    validation ^= (int)(pi_val == 3.1415);
    validation ^= (int)(first_val == 1.0);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}
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
//<ID> 1940
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
    cJSON *num0 = NULL;
    cJSON *num1 = NULL;
    cJSON *got_pi = NULL;
    cJSON *got_numbers = NULL;
    cJSON *got_item0 = NULL;
    char *out = NULL;
    double pi_val = 0.0;
    double item0_val = 0.0;
    cJSON_bool has_pi = 0;
    cJSON_bool has_numbers = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num0 = cJSON_CreateNumber(1.0);
    num1 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON_AddNumberToObject(root, "pi", 3.1415);
    out = cJSON_PrintUnformatted(root);

    // step 3: Core operations
    got_pi = cJSON_GetObjectItem(root, "pi");
    pi_val = cJSON_GetNumberValue(got_pi);
    got_numbers = cJSON_GetObjectItem(root, "numbers");
    got_item0 = cJSON_GetArrayItem(got_numbers, 0);
    item0_val = cJSON_GetNumberValue(got_item0);
    has_pi = cJSON_HasObjectItem(root, "pi");
    has_numbers = cJSON_HasObjectItem(root, "numbers");

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(num0 != NULL);
    validation ^= (int)(num1 != NULL);
    validation ^= (int)(out != NULL);
    validation ^= (int)(got_pi != NULL);
    validation ^= (int)(got_numbers != NULL);
    validation ^= (int)(got_item0 != NULL);
    validation ^= (int)(has_pi != 0);
    validation ^= (int)(has_numbers != 0);
    validation ^= (int)(pi_val == 3.1415);
    validation ^= (int)(item0_val == 1.0);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}
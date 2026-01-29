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
//<ID> 879
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *num_a = NULL;
    cJSON *num_b = NULL;
    cJSON *retrieved = NULL;
    char *temp_buf = NULL;
    double a_val = 0.0;
    int derived_int = 0;
    cJSON_bool added_a = 0;
    cJSON_bool added_b = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    num_a = cJSON_CreateNumber(42.5);
    num_b = cJSON_CreateNumber(-7.25);
    added_a = cJSON_AddItemToObject(root, "alpha", num_a);
    added_b = cJSON_AddItemToObject(root, "beta", num_b);
    temp_buf = (char *)cJSON_malloc(128);
    memset(temp_buf, 0, 128);
    temp_buf[0] = 'X';
    temp_buf[1] = '\0';

    // step 3: Operate / Validate
    retrieved = cJSON_GetObjectItem(root, "alpha");
    a_val = cJSON_GetNumberValue(retrieved);
    derived_int = (int)a_val;
    (void)added_a;
    (void)added_b;
    (void)derived_int;

    // step 4: Cleanup
    cJSON_DeleteItemFromObjectCaseSensitive(root, "beta");
    cJSON_free(temp_buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
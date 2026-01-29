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
//<ID> 494
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
    cJSON *n1 = (cJSON *)0;
    cJSON *n2 = (cJSON *)0;
    cJSON *nested = (cJSON *)0;
    cJSON *pi = (cJSON *)0;
    cJSON_bool ok = 0;
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();

    // step 3: Configure
    pi = cJSON_AddNumberToObject(root, "pi", 3.1415);
    n1 = cJSON_CreateNumber(7.0);
    n2 = cJSON_CreateNumber(13.0);
    ok = cJSON_AddItemToArray(arr, n1);
    ok = cJSON_AddItemToArray(arr, n2);
    ok = cJSON_AddItemToObject(root, "primes", arr);
    nested = cJSON_CreateObject();
    cJSON_AddNumberToObject(nested, "x", 100.0);
    ok = cJSON_AddItemToObject(root, "nested", nested);

    // step 4: Operate
    ok = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);

    // step 5: Validate
    (void)ok;
    (void)pi;
    (void)buffer;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}
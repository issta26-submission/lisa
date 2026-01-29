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
    // Create a root object
    cJSON *root = cJSON_CreateObject();
    if (!root) return -1;

    // Add some basic items
    cJSON_AddItemToObject(root, "pi", cJSON_CreateNumber(3.141592653589793));
    cJSON_AddItemToObject(root, "name", cJSON_CreateString("cJSON example"));
    cJSON_AddItemToObject(root, "enabled", cJSON_CreateTrue());

    // Create an array and add it to the root
    cJSON *primes = cJSON_CreateArray();
    if (!primes) { cJSON_Delete(root); return -2; }
    cJSON_AddItemToArray(primes, cJSON_CreateNumber(2));
    cJSON_AddItemToArray(primes, cJSON_CreateNumber(3));
    cJSON_AddItemToArray(primes, cJSON_CreateNumber(5));
    cJSON_AddItemToObject(root, "primes", primes);

    // Duplicate the array (deep copy) and attach it under a different key
    cJSON *primes_copy = cJSON_Duplicate(primes, /*recurse=*/1);
    if (!primes_copy) { cJSON_Delete(root); return -3; }
    cJSON_AddItemToObject(root, "primes_copy", primes_copy);

    // Print the JSON (unformatted) to a string
    char *printed = cJSON_PrintUnformatted(root);
    if (!printed) { cJSON_Delete(root); return -4; }

    // Parse the printed string back into a cJSON structure
    cJSON *parsed = cJSON_Parse(printed);
    if (!parsed) { free(printed); cJSON_Delete(root); return -5; }

    // Verify some values from the parsed structure
    cJSON *pi = cJSON_GetObjectItemCaseSensitive(parsed, "pi");
    if (!pi || !cJSON_IsNumber(pi)) {
        cJSON_Delete(parsed);
        free(printed);
        cJSON_Delete(root);
        return -6;
    }

    // Access the array and check size & first element
    cJSON *parsed_primes = cJSON_GetObjectItemCaseSensitive(parsed, "primes");
    if (!parsed_primes || !cJSON_IsArray(parsed_primes)) {
        cJSON_Delete(parsed);
        free(printed);
        cJSON_Delete(root);
        return -7;
    }
    int arr_size = cJSON_GetArraySize(parsed_primes);
    if (arr_size < 1) {
        cJSON_Delete(parsed);
        free(printed);
        cJSON_Delete(root);
        return -8;
    }
    cJSON *first_prime = cJSON_GetArrayItem(parsed_primes, 0);
    if (!first_prime || !cJSON_IsNumber(first_prime)) {
        cJSON_Delete(parsed);
        free(printed);
        cJSON_Delete(root);
        return -9;
    }

    // Detach the second element (index 1) from the parsed primes array and delete it
    cJSON *detached = cJSON_DetachItemFromArray(parsed_primes, 1);
    if (detached) {
        cJSON_Delete(detached);
    }

    // Make a writable copy of the printed JSON and minify it in-place
    size_t len = strlen(printed);
    char *min_copy = (char*)malloc(len + 1);
    if (min_copy) {
        memcpy(min_copy, printed, len + 1);
        cJSON_Minify(min_copy);
        free(min_copy);
    }

    // Duplicate the whole parsed tree (deep) and then delete the duplicate
    cJSON *parsed_dup = cJSON_Duplicate(parsed, /*recurse=*/1);
    if (parsed_dup) cJSON_Delete(parsed_dup);

    // Clean up
    free(printed);
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // Per requirement, return 66 on success
    return 66;
}